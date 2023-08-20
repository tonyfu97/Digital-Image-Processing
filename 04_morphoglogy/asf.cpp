/*
  Alternating Sequential Filters (ASF):
  imgIn : Input image
  n     : Maximum filter size
*/

#define cimg_use_png
#include "CImg.h"

using namespace cimg_library;

CImg<> SAFiltering(CImg<> &imgIn, int n)
{
    CImg<> imgOut(imgIn);
    for (int k = 1; k <= n; ++k)
    {
        // Structuring element of size k x k.
        CImg<> mask = CImg<>(k, k).fill(1);
        // Opening.
        imgOut.erode(mask).dilate(mask);
        // Closing.
        imgOut.dilate(mask).erode(mask);
    }
    return imgOut;
}

int main()
{
    CImg<unsigned char> img("../images/coins.png");

    // Convert to grayscale
    CImg<> lum = img.get_norm().blur(0.75f);

    // Thresholding
    lum.threshold(lum.mean() + 30).normalize(0, 255);

    //  Alternating Sequential Filtering:
    CImg<> imgSAFiltering = SAFiltering(lum, 3);
    imgSAFiltering.save_png("./results/coins_sa_filtering3.png");

    return 0;
}
