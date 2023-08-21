/*
  Morphological gradients.
  imgIn : Input image
  n     : size of the square structuring element
*/

#define cimg_use_png
#include "CImg.h"

using namespace cimg_library;

CImg<> MorphologicalGradients(CImg<> &imgIn, int n)
{
    CImg<> B = CImg<>(n, n).fill(1),
           imgErode = imgIn.get_erode(B),
           imgDilate = imgIn.get_dilate(B),
           imgOpening = imgErode.get_dilate(B),
           imgClosing = imgDilate.get_erode(B),
           gradE = imgIn - imgErode,
           gradD = imgDilate - imgIn;

    // Morphological gradients.
    gradE.get_normalize(0, 255).save_png("./results/coins_gradientE.png");
    gradD.get_normalize(0, 255).save_png("./results/coins_gradientD.png");

    // Beucher.
    CImg<> imgBeucher = imgDilate - imgErode;
    imgBeucher.normalize(0, 255).save_png("./results/coins_beucher.png");

    // Hop Hat.
    CImg<> whiteTopHat = imgIn - imgOpening,
           blackTopHat = imgClosing - imgIn;
    whiteTopHat.normalize(0, 255).save_png("./results/coins_white_top_hat.png");
    blackTopHat.normalize(0, 255).save_png("./results/coins_black_top_hat.png");

    // Edge detector.
    CImg<> contourMin = gradE.get_min(gradD),
           contourMax = gradE.get_max(gradD);
    contourMin.normalize(0, 255).save_png("./results/coins_contour_min.png");
    contourMax.normalize(0, 255).save_png("./results/coins_contour_max.png");

    // Nonlinear laplacian.
    CImg<> Laplician = gradD - gradE;
    Laplician.normalize(0, 255).save_png("./results/coins_laplacian.png");

    return Laplician;
}

int main()
{
    CImg<unsigned char> img("../images/coins.png");

    // Convert to grayscale
    CImg<> lum = img.get_norm().blur(0.75f);

    // Thresholding
    lum.threshold(lum.mean()+30).normalize(0, 255);

    // Morphological gradients:
    CImg<> imgMorphologicalGradients = MorphologicalGradients(lum, 3);

    return 0;
}