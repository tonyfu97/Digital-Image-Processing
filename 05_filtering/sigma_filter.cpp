/*
  Sigma filter (Adaptive filter based on the local variance)
  imgIn : Input image
*/

#define cimg_use_png
#include "CImg.h"

using namespace cimg_library;

CImg<> sigmaFilter(CImg<> &imgIn)
{
    CImgList<> g = imgIn.get_gradient();
    CImg<> grad = (g[0].get_sqr() + g[1].get_sqr()).sqrt();

    // Sum of the  gradients on a 3x3 neiçghborhood
    CImg<> Sgrad = grad.get_convolve(CImg<>(3, 3, 1, 1, 1));

    float epsilon = 100;
    CImg<> rap = imgIn.get_div(grad + epsilon),
           imgOut(imgIn);
        
    rap.get_normalize(0, 255).save("./results/lighthouse_rap.png");

    CImg_3x3(I, float); // declare Ipp, Ipc, etc.
    cimg_for3x3(rap, x, y, 0, 0, I, float)
        imgOut(x, y) = (Ipp + Ipc + Ipn + Icp + Icc + Icn + Inp + Inc + Inn) / (Sgrad(x, y) + epsilon);

    return imgOut;
}

int main()
{
    CImg<unsigned char> img("../images/lighthouse.png");

    // Convert to grayscale
    CImg<> lum = img.get_norm().blur(0.75f);

    // Add noise
    lum.noise(20, 0);
    lum.get_normalize(0, 255).save("./results/lighthouse_noise.png");

    CImg<> imgOut = sigmaFilter(lum);
    imgOut.normalize(0, 255).save("./results/lighthouse_sigma.png");
}