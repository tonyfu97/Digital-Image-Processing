/*
    Texture Spectrum (He and Wang, 1990)
*/

#define cimg_use_png
#include "CImg.h"

#include <string>

using namespace cimg_library;

unsigned char valE(float val1, float val2, float tau)
{
    return val1 < val2 - tau ? 0 : cimg::abs(val1 - val2) <= tau ? 1
                                                                 : 2;
}

CImg<> TextureUnit(CImg<> &imgIn)
{
    CImg<unsigned char> E(8);
    CImg<> N(imgIn);
    CImg_3x3(I, float);
    float tau = 5;
    cimg_for3x3(imgIn, x, y, 0, 0, I, float)
    {
        if (x > 0 && y > 0)
        {
            // The neighborhood is labeled counterclockwise
            E(0) = valE(Ipp, Icc, tau);
            E(1) = valE(Ipc, Icc, tau);
            E(2) = valE(Ipn, Icc, tau);
            E(3) = valE(Icn, Icc, tau);
            E(4) = valE(Inn, Icc, tau);
            E(5) = valE(Inc, Icc, tau);
            E(7) = valE(Icp, Icc, tau);
            E(6) = valE(Inp, Icc, tau);
            N(x, y) = E(0);
            for (int j = 1; j < 8; ++j)
                N(x, y) += E(j) * pow(3, j);
        }
    }
    return N;
}

int main()
{
    CImg<> imgIn("../images/farm.png");
    imgIn.norm().blur(0.75);
    CImg<> imgOut = TextureUnit(imgIn);
    imgOut.get_normalize(0, 255).save_png("./results/farm_texture_spectrum.png");

    // Compute the histogram with 256 bins
    CImg<unsigned int> histogram = imgOut.histogram(256);
    histogram.display_graph();

    return 0;
}
