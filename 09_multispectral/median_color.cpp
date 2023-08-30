/*
    Median filtering for color images
*/

#define cimg_use_png
#include "CImg.h"

#include <cmath>
#include <string>
#include <iostream>

using namespace cimg_library;

std::string imgName = "mondrian";

CImg<> ColorMedianFiltering(CImg<> imgIn)
{
    // Transformation into a perceptually uniform space
    imgIn.RGBtoLab();
    CImg<>
        imgOut(imgIn),
        V(5, 5, 1, 3), // 5x5 color neighbor
        vL = V.get_shared_channel(0),
        va = V.get_shared_channel(1),
        vb = V.get_shared_channel(2);
    cimg_for5x5(imgIn, x, y, 0, 0, vL, float)
    {
        cimg_get5x5(imgIn, x, y, 0, 1, va, float);
        cimg_get5x5(imgIn, x, y, 0, 2, vb, float);
        float dmin = 0; // Distance to the central pixel.
        CImg<> z = V.get_vector_at(2, 2);
        cimg_forXY(V, i, j)
        {
            CImg<> zi = V.get_vector_at(i, j);
            dmin += (zi -= z).magnitude();
        }
        cimg_forXY(V, i, j)
        {
            float d = 0;
            CImg<> z = V.get_vector_at(i, j);
            cimg_forXY(V, u, v)
            {
                CImg<> zi = V.get_vector_at(u, v);
                d += (zi -= z).magnitude();
            }
            if (d < dmin)
            {
                dmin = d;
                imgOut(x, y, 0, 0) = V(i, j, 0);
                imgOut(x, y, 0, 1) = V(i, j, 1);
                imgOut(x, y, 0, 2) = V(i, j, 2);
            }
        }
    }
    return imgOut.LabtoRGB();
}

int main()
{
    CImg<> imgIn(("../images/" + imgName + ".png").c_str());
    imgIn.resize(128, 128);
    imgIn.noise(5);
    imgIn.normalize(0, 255).save_png(("./results/" + imgName + "_noisy.png").c_str());
    // remove alpha channel
    if (imgIn.spectrum() == 4)
        imgIn = imgIn.get_channels(0, 2);

    CImg<> imgOut = ColorMedianFiltering(imgIn);
    imgOut.normalize(0, 255).save_png(("./results/median_color_" + imgName + ".png").c_str());

    return 0;
}
