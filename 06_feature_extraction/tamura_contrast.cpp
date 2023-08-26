/*
    Tamura contrast coefficients (Tamura et al., 1978)
*/

#define cimg_use_png
#include "CImg.h"

#include <string>
#include <iostream>

using namespace cimg_library;

/*
  Tamura’s contrast.
  imgIn : Input image
  n     : integer, power of the kurtosis
*/
float Contrast(CImg<> &imgIn, float n)
{
    float mean = imgIn.mean();
    float variance = imgIn.variance();

    // Kurtosis
    float kurtosis = 0;
    cimg_forXY(imgIn, x, y)
    {
        float diff = imgIn(x, y) - mean;
        kurtosis += cimg::sqr(cimg::sqr(diff));
    }
    kurtosis /= imgIn.size() * cimg::sqr(cimg::sqr(variance));

    return std::sqrt(variance) / std::pow(kurtosis, n);
}

int main()
{
    CImg<> imgIn("../images/car.png");

    float constrast_original = Contrast(imgIn, 0.5);
    std::cout << "Contrast: " << constrast_original << std::endl;

    float constrast_low_contrast = Contrast(imgIn.normalize(50, 200), 0.5);
    imgIn.save_png("./results/car_low_contrast.png");
    std::cout << "Contrast (low contrast): " << constrast_low_contrast << std::endl;
}