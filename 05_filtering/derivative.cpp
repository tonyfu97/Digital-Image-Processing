/*
  Horizontal Derivative filter
  imgIn : Input image
*/

#define cimg_use_png
#include "CImg.h"

using namespace cimg_library;

void Derivative(CImg<> &imageIn)
{
    // Gradient approximation using centered finite differences.
    CImgList<> grad = imageIn.get_gradient();
    grad[0].get_normalize(0, 255).save("./results/lighthouse_gradient_x.png");
    grad[1].get_normalize(0, 255).save("./results/lighthouse_gradient_y.png");

    // Norm and phase of the gradient.
    CImg<>
        norm = (grad[0].get_sqr() + grad[1].get_sqr()).sqrt(),
        phi = grad[1].get_atan2(grad[0]);

    norm.normalize(0, 255).save("./results/lighthouse_gradient_norm.png");
    phi.normalize(0, 255).save("./results/lighthouse_gradient_phi.png");
}

int main()
{
    CImg<unsigned char> img("../images/lighthouse.png");

    // Convert to grayscale
    CImg<> lum = img.get_norm().blur(0.75f);

    // Derivative:
    Derivative(lum);
}
