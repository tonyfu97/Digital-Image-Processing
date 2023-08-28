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
    dispField : Displacement field
    imgSeq    : Sequence of two images, stacked along z
    iterCount : Number of iterations for the numerical scheme
    alpha     : Regularization weight for the displacement field
*/
void HornSchunck(CImg<> &dispField, CImg<> &imgSeq, unsigned int iterCount, float alpha)
{
    CImgList<> grad = {imgSeq.get_slice(0).get_gradient("xy"), imgSeq.get_gradient("z", 1)};
    CImg<> avgKernel(3, 3, 1, 1, // Mask for averaging
                     0., 0.25, 0.,
                     0.25, 0., 0.25,
                     0., 0.25, 0.);
    CImg<> denom = grad[0].get_sqr() + grad[1].get_sqr() + 4 * alpha;

    for (unsigned int i = 0; i < iterCount; ++i)
    {
        CImg<> avgField = dispField.get_convolve(avgKernel);
        cimg_forXY(dispField, x, y)
        {
            float temp = (grad[0](x, y) * avgField(x, y, 0) + grad[1](x, y) * avgField(x, y, 1) + grad[2](x, y)) / denom(x, y);
            dispField(x, y, 0) = avgField(x, y, 0) - grad[0](x, y) * temp;
            dispField(x, y, 1) = avgField(x, y, 1) - grad[1](x, y) * temp;
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
