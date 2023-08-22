/*
  Median filter
  imgIn : Input image
  size  : Size of the filter
*/

#define cimg_use_png
#include "CImg.h"

using namespace cimg_library;

void MedianFilter(CImg<> &imgIn, int size)
{
    imgIn.blur_median(size);
}

int main()
{
    CImg<unsigned char> img("../images/coins.png");

    // Convert to grayscale
    CImg<> lum = img.get_norm().blur(0.75f);

    // Thresholding
    lum.threshold(lum.median()+30).normalize(0, 255);
    lum.save("./results/coins_threshold.png");

    // Apply the median filter with a given size
    int filterSize = 5;
    MedianFilter(lum, filterSize);
    lum.normalize(0, 255).save_png("./results/coins_median.png");

    return 0;
}
