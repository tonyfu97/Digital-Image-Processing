/*
    Tamura directionality coefficients (Tamura et al., 1978)
*/

#define cimg_use_png
#include "CImg.h"

#include <string>
#include <iostream>
#include <vector>

using namespace cimg_library;

/*
    Tamura’s directionality.
    imgIn : Input image
*/
float Directionality(CImg<> &imgIn)
{
    CImgList<> g = imgIn.get_gradient();
    CImg<>
        phi = g(1).get_atan2(g(0)),
        norm = (g[0].get_sqr() + g[1].get_sqr()).sqrt();
    float tau = 0.01f;
    cimg_forXY(phi, x, y)
        phi(x, y) = norm(x, y) > tau ? phi(x, y) : 0;
    CImg<> h = phi.get_histogram(90);
    h /= (imgIn.width() * imgIn.height());

    // Searching the maxima
    h.threshold(0.7 * h.max());
    int nb_pics = h.sum();

    // Location of the maxima
    CImg<int> perm;
    h.get_sort(perm, false);
    float D = 0;
    for (int p = 0; p < nb_pics; ++p)
        cimg_forX(h, x) D -= h(x) * (cimg::sqr(x - perm(p)));
    float r = 1;
    D *= r * nb_pics;
    return D + 1;
}

int main()
{
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
        "chequered"
    };

    for (auto &image : images)
    {
        CImg<> imgIn(("../textures/" + image + "1.png").c_str());
        imgIn.norm();
        float directionality = Directionality(imgIn);
        std::cout << "Directionality (" << image << "): " << directionality << std::endl;
    }

    return 0;
}