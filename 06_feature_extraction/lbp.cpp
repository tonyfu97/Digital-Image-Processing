/*
    Local Binary Pattern (LBP)
*/

#define cimg_use_png
#include "CImg.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>

using namespace cimg_library;

/*
  LBP and contrast - rotation invariant version
  imgIn : input image
  R     : neighborhood radius
  P     : number of neighbors
  lbp,C : output images
*/
void LBP(CImg<> &imgIn, float R, int p, CImg<> &lbp, CImg<> &C)
{
    float PI2 = 2 * cimg::PI;
    cimg_for_insideXY(imgIn, x, y, (int)(R + 1))
    {
        float
            Ibar = 0,
            Vc = imgIn(x, y);

        // Sampling p points on the circle of radius R
        CImg<> xi(p, 2, 1, 1, 0), V(p);
        cimg_forX(V, n)
        {
            xi(n, 0) = x - R * std::sin(PI2 * n / p);
            xi(n, 1) = y + R * std::cos(PI2 * n / p);
            V(n) = imgIn.linear_atXY(xi(n, 0), xi(n, 1));
            Ibar += V(n);
        }
        // Mean of the grey-levels
        Ibar /= p;

        // Computing U
        float U = 0;
        for (int n = 1; n < p; ++n)
        {
            float Vj = imgIn.linear_atXY(xi(n - 1, 0), xi(n - 1, 1));
            U += cimg::abs(V(n) - Vc > 0 ? 1 : 0) - cimg::abs(Vj - Vc > 0 ? 1 : 0);
        }

        // Fence post
        float
            Vi = imgIn.linear_atXY(xi(p - 1, 0), xi(p - 1, 1)),
            Vj = imgIn.linear_atXY(xi(0, 0), xi(0, 1));
        U += cimg::abs((Vi - Vc > 0 ? 1 : 0) - (Vj - Vc > 0 ? 1 : 0));

        // Computing the LBP
        if (U > 2)
            lbp(x, y) = p + 1;
        else
            cimg_forX(V, n)
                lbp(x, y) += (V(n) - Vc > 0 ? 1 : 0);
        
        // Computing contrast
        cimg_forX(V, n)
            C(x, y) += cimg::sqr(V(n) - Ibar);
        C(x, y) /= p;
    }
}

/*
    LBP concatenated histogram.
*/
CImg<> LBPHistogram(CImg<> &imgIn)
{
    int dx = imgIn.width(), dy = imgIn.height();
    // Number of patches in x and y.
    int nbX = 5, nbY = 5;
    // LBP and histogram parameters
    float R = 2;
    int p = 20, nbins = 20;
    // Concatenated histogram.
    CImg<> hglobal(nbins * nbX * nbY);
    for (int i = 0; i < nbX; ++i)
    {
        for (int j = 0; j < nbY; ++j)
        {
            CImg<>
                patch = imgIn.get_crop(i * dx / nbX, j * dy / nbY, 0, 0,
                                       (i + 1) * dx / nbX, (j + 1) * dy / nbY, 0, 0),
                lbp(patch.width(), patch.height(), 1, 1, 0),
                C(lbp);
            LBP(patch, R, p, lbp, C);
            CImg<> hlbp = lbp.get_histogram(nbins);
            cimg_forX(hlbp, x)
                hglobal((j + i * nbY) * nbins + x) = hlbp(x);
        }
    }
    return hglobal;
}

float h_distance(CImg<> &h1, CImg<> &h2)
{
    return (h1 - h2).get_abs().sum();
}

int main()
{
    CImg<> imgIn("../textures/grid1.png");  // change this to test other images
    imgIn.norm();
    CImg<> h1 = LBPHistogram(imgIn);

    std::vector<std::string> images = {
        "cracked",
        "banded",
        "cobwebbed",
        "dotted",
        "bubbly",
        "fibrous",
        "honeycombed",
        "grid",
        "spiralled",
        "chequered",
        "wrinkled",
        "braided",
    };

    std::map<float, std::string> distance_map;
    std::ofstream outfile("./results/lbp.txt");

    for (auto &image : images)
    {
        CImg<> imgCurrent(("../textures/" + image + "2.png").c_str());
        imgCurrent.norm();
        CImg<> h2 = LBPHistogram(imgCurrent);
        float distance = h_distance(h1, h2);

        distance_map[distance] = image;

        outfile << "Distance (" << image << "): " << distance << std::endl;
    }

    outfile.close();

    std::cout << "Ranking of the images:" << std::endl;
    int rank = 1;
    for (auto &item : distance_map)
    {
        std::cout << "Rank " << rank << ": " << item.second << " with distance " << item.first << std::endl;
        ++rank;
    }

    return 0;
}
