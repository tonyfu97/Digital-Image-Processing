/*
    Gaussian (Frequency-Domain) Filter
*/

#define cimg_use_png
#include "CImg.h"

using namespace cimg_library;

CImg<> GaussianFilter(CImg<> &imgIn, float sigma)
{
    // FFT
    CImgList<> fImg = imgIn.get_FFT();
    // Frequency response of the filter.
    CImg<> gaussMask(imgIn.width(), imgIn.height());
    float sigma2 = cimg::sqr(sigma);
    cimg_forXY(gaussMask, x, y)
    {
        float fx = x / (float)imgIn.width() - 0.5f, fx2 = cimg::sqr(fx),
              fy = y / (float)imgIn.height() - 0.5f,
              fy2 = cimg::sqr(fy);
        gaussMask(x, y) = 2 * cimg::PI * sigma2 *
                          std::exp(-2 * cimg::sqr(cimg::PI) * sigma2 * (fx2 + fy2));
    }
    // Zero shift.
    gaussMask.shift(-imgIn.width() / 2, -imgIn.height() / 2, 0, 0, 2);
    // Filtering
    cimglist_for(fImg, k)
        fImg[k].mul(gaussMask);
    // Inverse FFT and real part.
    return fImg.get_FFT(true)[0].normalize(0, 255);
}

int main()
{
    CImg<unsigned char> img("../images/lighthouse.png");

    // Convert to grayscale
    CImg<> lum = img.get_norm().blur(0.75f);

    // Resize to the nearest power of 2 dimensions
    int width = 1 << static_cast<int>(std::ceil(std::log2(lum.width())));
    int height = 1 << static_cast<int>(std::ceil(std::log2(lum.height())));
    lum.resize(1024, 512);

    // Gaussian filter
    CImg<> imgOut = GaussianFilter(lum, 2);
    // imgOut.resize(width, height);

    imgOut.normalize(0, 255).save("./results/lighthouse_gaussian.png");

    return 0;
}