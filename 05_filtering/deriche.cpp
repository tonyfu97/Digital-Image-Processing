/*
  Deriche filter on a 1D signal (along the X axis).
    imgIn : Input image
    alpha : Scale parameter
    order : Filter order
            (0: smoothing, 1: first derivative, 2: second derivative)
    boundary_conditions : Boundary conditions
*/

#define cimg_use_png
#include "CImg.h"

using namespace cimg_library;

const float SIGMA = 3.0f;

CImg<> deriche0(CImg<> &imgIn)
{
    CImg<> img_deriche0 = imgIn.get_deriche(SIGMA, 0, 'x');
    img_deriche0.deriche(SIGMA, 0, 'y');
    return img_deriche0;
}

CImg<> deriche1_norm(CImg<> &imgIn)
{
    CImg<> img_deriche1 = imgIn.get_deriche(SIGMA, 1, 'x');
    img_deriche1.deriche(SIGMA, 1, 'y');
    return (img_deriche1.get_sqr() += img_deriche1.get_sqr()).sqrt();
}

CImg<> deriche2(CImg<> &imgIn)
{
    CImg<> img_deriche2_x = imgIn.get_deriche(SIGMA, 2, 'x');
    CImg<> img_deriche2_y = imgIn.get_deriche(SIGMA, 2, 'y');
    return img_deriche2_x + img_deriche2_y;
}

int main()
{
    CImg<unsigned char> img("../images/lighthouse.png");

    // Convert to grayscale
    CImg<> lum = img.get_norm().blur(0.75f);

    // Deriche smooth
    CImg<> img_deriche0 = deriche0(lum);
    img_deriche0.normalize(0, 255).save("./results/lighthouse_deriche0.png");

    // Deriche first derivative
    CImg<> img_deriche1 = deriche1_norm(lum);
    img_deriche1.normalize(0, 255).save("./results/lighthouse_deriche1.png");

    // Deriche second derivative
    CImg<> img_deriche2 = deriche2(lum);
    img_deriche2.normalize(0, 255).save("./results/lighthouse_deriche2.png");

    return 0;
}