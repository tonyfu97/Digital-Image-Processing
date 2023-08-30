/*
    Simple 3D example
*/

#define cimg_use_png
#include "CImg.h"

#include <cmath>
#include <string>
#include <iostream>

using namespace cimg_library;

int main()
{
    // Structures of the 3D object to be generated.
    CImg<> g_points;
    CImgList<unsigned int> g_primitives;
    // Add torii in chain.
    CImgList<unsigned int> primitives;
    CImg<> points;
    for (unsigned int k = 0; k < 8; ++k)
    {
        points = CImg<>::torus3d(primitives, 100, 30);
        points.resize_object3d(240, 160, 60).shift_object3d(130.f * k, 0.f, 0.f);
        if (k % 2)
            points.rotate_object3d(1, 0, 0, 90);
        g_points.append_object3d(g_primitives, points, primitives);
    }
    // Add a sphere at the end of the chain.
    points = CImg<>::sphere3d(primitives, 150).shift_object3d(1000, 0, 0);
    g_points.append_object3d(g_primitives, points, primitives);
    // Create a 800x600 background image, with color gradients.
    CImg<unsigned char> background(1, 2, 1, 3, "y?[64,128,255]:[255,255,255]", false);
    background.resize(800, 600, 1, 3, 3);
    // Launch the 3D interactive viewer.
    background.display_object3d("3D Object", g_points, g_primitives);
}
