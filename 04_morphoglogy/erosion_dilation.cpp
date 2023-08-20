#define cimg_use_png
#include "CImg.h"
#include <iostream>

using namespace cimg_library;

int main()
{
    CImg<unsigned char> img("../images/coins.png");

    // Convert to grayscale
    CImg<> lum = img.get_norm().blur(0.75f).normalize(0, 255);
    lum.save("./results/coins_grayscale.png");

    // Thresholding
    lum.threshold(lum.median()).normalize(0, 255);
    lum.save("./results/coins_threshold.png");

    // Structuring element. Hear a 3*3 square.
    CImg<unsigned char> B = CImg<unsigned char>(3, 3).fill(1);
    CImg<unsigned char>
        imgErode = lum.get_erode(B),   // Erosion
        imgDilate = lum.get_dilate(B); // Dilation"

    // Save images
    imgErode.save_png("./results/coins_erosion.png");
    imgDilate.save_png("./results/coins_dilation.png");

    return 0;
}
