/*
  Horizontal Sobel filter
  imgIn : Input image
*/

#define cimg_use_png
#include "CImg.h"

using namespace cimg_library;

void SobelConv(CImg<> &imgIn)
{
    CImg<> sobel(3, 3, 1, 1, 0);
    sobel(0, 0) = -1;
    sobel(0, 1) = -2;
    sobel(0, 2) = -1;
    sobel(1, 0) = 0;
    sobel(1, 1) = 0;
    sobel(1, 2) = 0;
    sobel(2, 0) = 1;
    sobel(2, 1) = 2;
    sobel(2, 2) = 1;

    imgIn.convolve(sobel);
}

int main()
{
    CImg<unsigned char> img("../images/lighthouse.png");

    // Convert to grayscale
    CImg<> lum = img.get_norm().blur(0.75f);

    // Thresholding
    lum.threshold(lum.mean() + 30).normalize(0, 255);

    // Convolution:
    SobelConv(lum);
    lum.normalize(0, 255).save_png("./results/lighthouse_conv.png");

    return 0;
}
