/*
    Bernsen's algorithm
*/

#define cimg_use_png
#include "CImg.h"

#include <string>
#include <iostream>
#include <algorithm>

using namespace cimg_library;

/*
    Bernsen algorithm.
    imgIn : input image
    cmin  : lower bound for the contrast
*/
CImg<> Bernsen(CImg<> &imgIn, float cmin)
{
    // Create an output image with the same dimensions as the input
    CImg<> imgOut(imgIn.width(), imgIn.height(), 1, 1, 0);

    // Initialize the value for thresholding
    float thresholdValue = 0;

    // Create a 5x5 neighborhood for local processing
    CImg<> neighborhood(5, 5);

    // Loop through the input image using a 5x5 window
    cimg_for5x5(imgIn, x, y, 0, 0, neighborhood, float)
    {
        // Find the minimum and maximum pixel values in the 5x5 window
        float minPixelValue, maxPixelValue;
        maxPixelValue = neighborhood.max_min(minPixelValue);

        // Apply Bernsen's algorithm
        if (maxPixelValue - minPixelValue > cmin)
        {
            imgOut(x, y) = (maxPixelValue + minPixelValue) / 2;
        }
        else
        {
            imgOut(x, y) = thresholdValue;
        }
    }

    return imgOut;
}

int main(int argc, char *argv[])
{
    CImg<> img("../images/mountain.png");
    img.norm().blur(0.75f);
    img.normalize(0, 255).save_png("./results/bernsen_input.png");

    CImg<> img_segmented = Bernsen(img, 40.0f);
    img_segmented.normalize(0, 255).save_png("./results/bernsen_output.png");

    img_segmented.threshold(1).normalize(0, 255).save_png("./results/bernsen_threshold.png");

    return 0;
}
