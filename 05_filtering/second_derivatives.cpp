/*
  Second-order derivatives
  imgIn : Input image
*/

#define cimg_use_png
#include "CImg.h"

using namespace cimg_library;

void Laplacian(CImg<> &imageIn)
{
    CImg<> laplacian = imageIn.get_laplacian();
    laplacian.normalize(0, 255).save("./results/lighthouse_laplacian.png");
}

void Hessian(CImg<> &imageIn)
{
    CImg<> Ixx = imageIn.get_hessian("xx")[0];
    CImg<> Ixy = imageIn.get_hessian("xy")[0];
    CImg<> Iyy = imageIn.get_hessian("yy")[0];

    Ixx.normalize(0, 255).save("./results/lighthouse_hessian_xx.png");
    Ixy.normalize(0, 255).save("./results/lighthouse_hessian_xy.png");
    Iyy.normalize(0, 255).save("./results/lighthouse_hessian_yy.png");
}

void LoG(CImg<> &imageIn)
{
    CImg<> log = imageIn.get_blur(2).laplacian();
    log.normalize(0, 255).save("./results/lighthouse_log.png");
}

void DoG(CImg<> &imageIn)
{
    CImg<> gauss1 = imageIn.get_blur(1);
    CImg<> gauss2 = imageIn.get_blur(2);
    CImg<> dog = gauss1 - gauss2;
    dog.normalize(0, 255).save("./results/lighthouse_dog.png");
}

int main()
{
    CImg<unsigned char> img("../images/lighthouse.png");

    // Convert to grayscale
    CImg<> lum = img.get_norm().blur(0.75f);

    // Second-order derivatives:
    Laplacian(lum);
    Hessian(lum);
    LoG(lum);
    DoG(lum);
}
