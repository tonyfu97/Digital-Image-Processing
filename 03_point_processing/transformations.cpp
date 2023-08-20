/*
    Testing a few point processing transformations.
*/

#include <string>

#define cimg_use_png
#include "CImg.h"

using namespace cimg_library;

int main(int argc, char **argv)
{
    // Read image filename from command line or use default
    cimg_usage("Decomposition into blocks of a color image");
    std::string filename = cimg_option("-i", "../images/lighthouse.png", "Color image");
    std::string file_basename = filename.substr(9, filename.size() - 13);

    // Read image
    CImg<> img(filename.c_str());
    CImg<> lum = img.get_norm().blur(0.75f).normalize(0, 255);
    lum.normalize(0, 255).save(("results/" + file_basename + "_original.png").c_str());

    // 1. exp(I)/50
    CImg<> expImg = lum.get_exp()/50;
    expImg.save(("results/" + file_basename + "_exp.png").c_str());

    // 2. 10 * sqrt(I)
    CImg<> sqrtImg = lum.get_sqrt()*10;
    sqrtImg.normalize(0, 255).save(("results/" + file_basename + "_sqrt.png").c_str());

    // 3. log(1 + |I|)
    CImg<> logImg = (2 + lum.get_abs()).log();
    logImg.normalize(0, 255).save(("results/" + file_basename + "_log.png").c_str());

    // 4. I^3
    CImg<> cubeImg = lum.get_pow(3);
    cubeImg.normalize(0, 255).save(("results/" + file_basename + "_cube.png").c_str());

    return 0;
}