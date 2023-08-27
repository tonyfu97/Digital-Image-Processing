/*
    Otsu's algorithm (histogram-based segmentation)
*/

#define cimg_use_png
#include "CImg.h"

#include <string>
#include <iostream>
#include <algorithm>

using namespace cimg_library;

/*
  Otsu algorithm
  imgIn     : input image
  nb_levels : number of bins
*/
float Otsu(CImg<> &imgIn, int nb_levels)
{
    float sigmaBmax = 0;
    long n0 = 0, n1;
    int th = -1;
    CImg<> hist = imgIn.get_histogram(nb_levels);
    cimg_forX(hist, i)
    {
        if (i < nb_levels - 1)
        // If i==nb_levels - 1, all the pixels belong to class 0.
        {
            n0 += hist[i];
            n1 = imgIn.size() - n0;
            float mu0 = 0, mu1 = 0;
            for (int j = 0; j <= i; ++j)
                mu0 += j * hist[j];
            for (int j = i + 1; j < nb_levels; ++j)
                mu1 += j * hist[j];
            mu0 /= n0;
            mu1 /= n1;
            if (n0 * n1 > 0)
            {
                float sigmaB = n0 * n1 * cimg::sqr(mu0 - mu1) / cimg::sqr(imgIn.size());
                if (sigmaB > sigmaBmax)
                {
                    sigmaBmax = sigmaB;
                    th = i;
                }
            }
        }
    }
    return (float)th;
}

int main(int argc, char *argv[])
{
    cimg_usage("Otsu's algorithm");
    
}