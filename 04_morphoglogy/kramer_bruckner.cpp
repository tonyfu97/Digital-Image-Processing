/*
  Kramer-Bruckner filter.
  imgIn : Input image
  n     : size of the square structuring element
*/

#define cimg_use_png
#include "CImg.h"

using namespace cimg_library;

CImg<> KramerBruckner(CImg<> &imgIn, int n)
{
    CImg<>
        imgOut(imgIn),
        mask = CImg<>(n, n).fill(1),
        imgErode = imgIn.get_erode(mask),
        imgDilate = imgIn.get_dilate(mask); // Dilation
    cimg_forXY(imgOut, x, y)
    {
        float M = 0.5f * (imgErode(x, y) + imgDilate(x, y));
        imgOut(x, y) = (imgIn(x, y) <= M ? imgErode(x, y) : imgDilate(x, y));
    }
    return imgOut;
}

int main()
{
    CImg<unsigned char> img("../images/coins.png");

    // Convert to grayscale
    CImg<> lum = img.get_norm().blur(0.75f);

    // Thresholding
    lum.threshold(lum.mean()+30).normalize(0, 255);

    // Kramer-Bruckner filter
    CImg<> imgKramerBruckner = KramerBruckner(lum, 3);
    imgKramerBruckner.save_png("./results/coins_kramer_bruckner.png");

    return 0;
}
