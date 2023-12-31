/*
    Horn-Schunck optical flow method
*/

#define cimg_use_png
#include "CImg.h"

#include <cmath>
#include <string>
#include <iostream>

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

int main()
{
    CImg<>
        img1("../images/driveby_1.png"),
        img2("../images/driveby_2.png");

    img1.norm().blur(1.0f);
    img2.norm().blur(1.0f).resize(img1);

    // Sequence of two images, stacked along z
    CImg<> seq(img1.width(), img1.height(), 2, 1, 0.);
    seq.draw_image(0, 0, 0, 0, img1);
    seq.draw_image(0, 0, 1, 0, img2);

    CImg<> V(img1.width(), img1.height(), 1, 2, 0.);
    HornSchunck(V, seq, 100, 0.1f);
    V.norm().normalize(0, 255).save_png("./results/horn_schunck.png");

    return 0;
}
