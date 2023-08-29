/*
    Lucas-Kanade optical flow (with eigenelement analysis)
*/

#define cimg_use_png
#include "CImg.h"

#include <cmath>
#include <string>
#include <iostream>

using namespace cimg_library;

/*
    Lucas-Kanade method with eigenelement analysis
    seq: Sequence of two images, stacked along z
*/
CImg<> LucasKanadeEigen(CImg<> &seq) {
    // Define window size and half-window size
    int windowSize = 9, halfWindowSize = windowSize / 2;

    // Initialize matrices and eigenvalue list
    CImg<> AtA(2, 2), Atb(1, 2), windowFunction(windowSize, windowSize);
    CImgList<> eigenValuesAndVectors;

    // Eigenvalue thresholds
    float epsilon = 1e-8f, tau_D = 300;

    // Initialize the velocity field
    CImg<> velocityField(seq.width(), seq.height(), 1, 2, 0);

    // Compute image gradients
    CImgList<> gradient = seq.get_gradient("xyz", 4);

    // Generate Gaussian window function
    float sigma = 10, color = 1;
    windowFunction.draw_gaussian(halfWindowSize, halfWindowSize, sigma, &color);

    // Loop over the image pixels
    cimg_for_insideXY(seq, x, y, windowSize) {
        // Reset matrices
        AtA.fill(0);
        Atb.fill(0);

        // Compute AtA and Atb matrices using window
        for (int dx = -halfWindowSize; dx <= halfWindowSize; ++dx) {
            for (int dy = -halfWindowSize; dy <= halfWindowSize; ++dy) {
                float weight = cimg::sqr(windowFunction(dx + halfWindowSize, dy + halfWindowSize));

                float Ix = gradient(0, x + dx, y + dy);
                float Iy = gradient(1, x + dx, y + dy);
                float It = gradient(2, x + dx, y + dy);

                AtA(0, 0) += weight * Ix * Ix;
                AtA(1, 1) += weight * Iy * Iy;
                AtA(0, 1) += weight * Ix * Iy;
                Atb(0)    += weight * Ix * It;
                Atb(1)    += weight * Iy * It;
            }
        }
        AtA(1, 0) = AtA(0, 1);

        // Eigenvalue analysis
        eigenValuesAndVectors = AtA.get_symmetric_eigen();
        CImg<> &eigenValues = eigenValuesAndVectors[0];

        // Invert AtA
        AtA.invert();

        // Check eigenvalue conditions and compute velocity
        if (std::min(eigenValues(0), eigenValues(1)) >= tau_D) {
            CImg<> localVelocity = -AtA * Atb;
            velocityField(x, y, 0, 0) = localVelocity(0);
            velocityField(x, y, 0, 1) = localVelocity(1);
        } else if (eigenValues(1) >= tau_D && cimg::abs(AtA.det()) > epsilon) {
            CImg<> localVelocity = -AtA * Atb;
            const float projection = localVelocity(0) * eigenValuesAndVectors(1, 0) + localVelocity(1) * eigenValuesAndVectors(0, 0);
            velocityField(x, y, 0, 0) = -projection * eigenValuesAndVectors(1, 0);
            velocityField(x, y, 0, 1) = projection * eigenValuesAndVectors(0, 0);
        }
    }
    return velocityField;
}

int main()
{
    std::string image_name = "driveby";
    CImg<>
        img1(("../docs/images/" + image_name + "_1.png").c_str()),
        img2(("../docs/images/" + image_name + "_2.png").c_str());

    img1.normalize(0, 255).blur(1.0f);
    img2.normalize(0, 255).blur(1.0f).resize(img1);

    // Remove alpha channel
    if (img1.spectrum() == 4)
        img1.channels(0, 2);
    if (img2.spectrum() == 4)
        img2.channels(0, 2);

    // Sequence of two images, stacked along z
    CImg<> seq(img1.width(), img1.height(), 2, 1);
    seq.draw_image(0, 0, 0, 0, img1);
    seq.draw_image(0, 0, 1, 0, img2);

    CImg<> V = LucasKanadeEigen(seq);

    unsigned char color[] = {255, 0, 0};
    img1.draw_quiver(V, color, 0.5f, 10, 10, true);
    img1.normalize(0, 255).save_png(("./results/lucas_kanade_eigen_" + image_name + ".png").c_str());

    return 0;
}