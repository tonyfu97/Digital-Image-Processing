/*
  Adaptive window filter
  imgIn : Input image
*/

#define cimg_use_png
#include "CImg.h"

using namespace cimg_library;

CImg<> Nagao(CImg<> &imgIn)
{
    CImg<> imgOut(imgIn);
    CImgList<unsigned char> Nagao(9, 5, 5, 1, 1, 0);
    Nagao(0, 0, 0) = Nagao(0, 0, 1) = Nagao(0, 0, 2) = Nagao(0, 0, 3) =
        Nagao(0, 0, 4) = Nagao(0, 1, 1) = Nagao(0, 1, 2) = Nagao(0, 1, 3) =
            Nagao(0, 2, 2) = 1;
    for (int i = 1; i < 4; ++i)
        Nagao[i] = Nagao[0].get_rotate(i * 90);

    Nagao(4, 1, 1) = Nagao(4, 1, 2) = Nagao(4, 1, 3) = Nagao(4, 2, 1) =
        Nagao(4, 2, 2) = Nagao(4, 2, 3) = Nagao(4, 3, 1) = Nagao(4, 3, 2) =
            Nagao(4, 3, 3) = 1;

    Nagao(5, 0, 0) = Nagao(5, 0, 1) = Nagao(5, 0, 2) = Nagao(5, 1, 0) =
        Nagao(5, 1, 1) = Nagao(5, 1, 2) = Nagao(5, 2, 0) = Nagao(5, 2, 1) =
            Nagao(5, 2, 2) = 1;
    for (int i = 1; i < 4; ++i)
        Nagao[5 + i] = Nagao[5].get_rotate(i * 90);

    // Neighborhood analysis.
    CImg<>
        mu(9, 1, 1, 1, 0),
        sigma(9, 1, 1, 1, 0),
        st,
        N(5, 5);
    CImg<int> permutations;
    cimg_for5x5(imgIn, x, y, 0, 0, N, float)
    {
        CImgList<> res(9);
        for (int i = 0; i < 9; ++i)
        {
            res[i] = N.get_mul(Nagao[i]);
            st = res[i].get_stats();
            mu[i] = st[2];
            sigma[i] = st[3];
        }
        // Searching minimal variance.
        sigma.sort(permutations);
        imgOut(x, y) = mu[permutations[0]];
    }
    return imgOut;
}

CImg<> Kuwahara(CImg<> &imgIn)
{
    CImg<> imgOut(imgIn);

    CImgList<unsigned char> Kuwahara(4, 5, 5, 1, 1, 0);
    cimg_for_inXY(Kuwahara[0], 0, 0, 2, 2, i, j) Kuwahara(0, i, j) = 1;

    for (int i = 1; i < 4; ++i)
        Kuwahara[i] = Kuwahara[0].get_rotate(i * 90);

    // Neighborhood analysis.
    CImg<>
        mu(9, 1, 1, 1, 0),
        sigma(9, 1, 1, 1, 0),
        st,
        N(5, 5);
    CImg<int> permutations;
    cimg_for5x5(imgIn, x, y, 0, 0, N, float)
    {
        CImgList<> res(4);
        for (int i = 0; i < 4; ++i)
        {
            res[i] = N.get_mul(Kuwahara[i]);
            st = res[i].get_stats();
            mu[i] = st[2];
            sigma[i] = st[3];
        }
        sigma.sort(permutations);
        imgOut(x, y) = mu[permutations[0]];
    }
    return imgOut;
}

int main()
{
    CImg<unsigned char> img("../images/lighthouse.png");

    // Convert to grayscale
    CImg<> lum = img.get_norm().blur(0.75f);

    // Add noise
    lum.noise(40, 0);
    lum.get_normalize(0, 255).save("./results/lighthouse_noise40.png");

    CImg<> imgOut = Nagao(lum);
    imgOut.normalize(0, 255).save("./results/lighthouse_nagao.png");

    imgOut = Kuwahara(lum);
    imgOut.normalize(0, 255).save("./results/lighthouse_kuwahara.png");

    return 0;
}