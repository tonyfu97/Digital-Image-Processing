#define cimg_use_png
#include "CImg.h"
#include <iostream>

using namespace cimg_library;

int main()
{
    CImg<unsigned char> img("../images/coins.png");

    // Convert to grayscale
    CImg<> lum = img.get_norm().blur(0.75f);

    // Thresholding
    lum.threshold(lum.median()).normalize(0, 255);

    // Structuring element. Hear a 3*3 square.
    CImg<unsigned char> B = CImg<unsigned char>(3, 3).fill(1);
    CImg<unsigned char>
        imgErode = lum.get_erode(B),   // Erosion
        imgDilate = lum.get_dilate(B), // Dilation"
        imgOpen = imgErode.get_dilate(B), // Opening
        imgClose = imgDilate.get_erode(B); // Closing

    // Save images
    imgOpen.save_png("./results/coins_opening.png");
    imgClose.save_png("./results/coins_closing.png");

    return 0;
}
