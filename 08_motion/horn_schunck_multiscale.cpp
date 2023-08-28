/*
    Horn-Schunck optical flow method (with multiscale resolution)
*/

#define cimg_use_png
#include "CImg.h"

#include <cmath>
#include <string>
#include <iostream>
#include <algorithm>

using namespace cimg_library;

/*
    Horn and Schunck method
    V       : Displacement field
    seq     : Sequence of two images, stacked along z
    nb_iters: Number of iterations for the numerical scheme
    alpha   : Regularization weight for the displacement field
*/
void HornSchunck(CImg<> &displacementField,
                 CImg<> &imageSequence,
                 unsigned int numIterations,
                 float regularizationAlpha)
{
    CImgList<> gradients = (imageSequence.get_slice(0).get_gradient("xy"), imageSequence.get_gradient("z", 1));
    CImg<> avgKernel(3, 3, 1, 1,
                     0., 0.25, 0.,
                     0.25, 0., 0.25,
                     0., 0.25, 0.);
    CImg<> denom = gradients[0].get_sqr() + gradients[1].get_sqr() + 4 * regularizationAlpha;

    for (unsigned int iter = 0; iter < numIterations; ++iter)
    {
        CImg<> averagedField = displacementField.get_convolve(avgKernel);

        cimg_forXY(displacementField, x, y)
        {
            float tempTerm = (gradients[0](x, y) * averagedField(x, y, 0) + gradients[1](x, y) * averagedField(x, y, 1) + gradients[2](x, y)) / denom(x, y);
            displacementField(x, y, 0) = averagedField(x, y, 0) - gradients[0](x, y) * tempTerm;
            displacementField(x, y, 1) = averagedField(x, y, 1) - gradients[1](x, y) * tempTerm;
        }
    }
}

// For multi-scale resolution
void ComputeMultiScale(CImg<> &seq, CImg<> &V, int nb_scales, int nb_iters, float alpha)
{
    for (int N = nb_scales - 1; N >= 0; --N)
    {
        float scaleFactor = std::pow(2, static_cast<double>(N));
        int
            scaledWidth = std::max(1, static_cast<int>(seq.width() / scaleFactor)),
            scaledHeight = std::max(1, static_cast<int>(seq.height() / scaleFactor));
        
        CImg<> scaledSeq = seq.get_resize(scaledWidth, scaledHeight).blur(N, N, 0, true, false);

        if (V)
            (V *= 2).resize(scaledWidth, scaledHeight, 1, 2);
        else
            V.assign(scaledWidth, scaledHeight, 1, 2, 0);

        HornSchunck(V, scaledSeq, nb_iters << N, alpha);
    }
}

int main()
{
    CImg<>
        img1("../docs/images/driveby_1.png"),
        img2("../docs/images/driveby_2.png");

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

    CImg<> V(img1.width(), img1.height(), 1, 2, 0.);
    int nb_scales = 1;
    int nb_iters = 100;
    float alpha = 0.1f; // Regularization weight

    ComputeMultiScale(seq, V, nb_scales, nb_iters, alpha);

    unsigned char color[] = {255, 0, 0};
    img1.draw_quiver(V, color, 0.5f, 20, 1, true);
    img1.save_png("./results/horn_schunck_multiscale.png");

    return 0;
}