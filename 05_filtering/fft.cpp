/*
    Fast Fourier Transform Demo
*/

#define cimg_use_png
#include "CImg.h"

using namespace cimg_library;

int main()
{
    CImg<unsigned char> img("../images/lighthouse.png");

    // Convert to grayscale
    CImg<> lum = img.get_norm().blur(0.75f);

    // Resize to the nearest power of 2 dimensions
    int width = 1 << static_cast<int>(std::ceil(std::log2(lum.width())));
    int height = 1 << static_cast<int>(std::ceil(std::log2(lum.height())));
    lum.resize(width, height, -100, -100, 0);

    // FFT
    CImgList<> fft = lum.get_FFT();
    // Log-transform the magnitude
    CImg<> magnitude(fft[0]);
    magnitude += 1; // Avoid log(0)
    magnitude.log();

    // Rearrange the data (place the zero in the middle).
    magnitude.shift(magnitude.width() / 2, magnitude.height() / 2, 0, 0, 2);

    magnitude.normalize(0, 255).save("./results/lighthouse_fft_mag.png");
    fft[1].get_normalize(0, 255).save("./results/lighthouse_fft_phase.png");

    // Inverse FFT
    CImg<> img_ifft = fft.get_FFT(true)[0];
    img_ifft.normalize(0, 255).save("./results/lighthouse_ifft.png");

    return 0;
}
