#include <string>
#include <cmath>

#define cimg_use_png
#include <CImg.h>

using namespace cimg_library;

int main(int argc, char **argv)
{
    // Read image filename from command line or use default
    cimg_usage("Apply different transformations to an image");
    std::string filename = cimg_option("-i", "../images/lighthouse.png", "Color image");
    std::string file_basename = filename.substr(9, filename.size() - 13);

    // Load image
    CImg<unsigned char> img(filename.c_str());

    // Convert it to luminance and smooth it
    CImg<> lum = img.get_norm().blur(1.0f).normalize(0, 255);
    lum.save(("results/" + file_basename + "_lum.png").c_str());

    // 1. Double intensity
    CImg<unsigned char> img_double = lum;
    img_double.fill("2*I", true);
    img_double.save(("results/" + file_basename + "_double.png").c_str());

    // 2. Inverting
    CImg<unsigned char> img_invert = lum;
    img_invert.fill("255-I", true);
    img_invert.save(("results/" + file_basename + "_invert.png").c_str());

    // 3. Spiral
    CImg<unsigned char> img_spiral = lum;
    img_spiral.fill("(x*y)%500",true);
    img_spiral.save(("results/" + file_basename + "_spiral.png").c_str());

    // 4. Conditional Operations
    CImg<unsigned char> img_conditional = lum;
    img_conditional.fill("I*(I!=J(1,0) || I!=J(0,1)?0:1)", true);
    img_conditional.save(("results/" + file_basename + "_conditional.png").c_str());

    return 0;
}
