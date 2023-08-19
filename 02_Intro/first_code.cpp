#define cimg_use_png
#include "CImg.h"
using namespace cimg_library;

int main()
{
    CImg<unsigned char> img("../images/lighthouse.png");
    img.display("Lighthouse");

    return 0;
}