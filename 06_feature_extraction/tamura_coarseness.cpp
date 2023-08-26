/*
    Tamura coarseness coefficients (Tamura et al., 1978)
*/

#define cimg_use_png
#include "CImg.h"

#include <string>
#include <iostream>

using namespace cimg_library;

/*
  Integral image of a point.
  imgInt : integral image
  x,y    : pixel
  k      : size of the window
*/
float IntegralMean(CImg<> &imgInt, int x, int y, int k)
{
    int
        k2 = k / 2,
        startx = std::max(0, x - k2),
        starty = std::max(0, y - k2),
        stopx = std::min(imgInt.width() - 1, x + k2 - 1),
        stopy = std::min(imgInt.height() - 1, y + k2 - 1);
    float
        l1 = startx - 1 < 0 ? 0 : imgInt(startx - 1, stopy, 0),
        l2 = starty - 1 < 0 ? 0 : imgInt(stopx, starty - 1, 0),
        l3 = starty - 1 < 0 || startx - 1 < 0 ? 0 : imgInt(startx - 1, starty - 1, 0),
        l4 = imgInt(stopx, stopy, 0);
    return (l4 - l1 - l2 + l3) /
           ((stopy - starty + 1) * (stopx - startx + 1));
}

/*
    Ak coefficients.
    imgIn : Input image
*/
CImg<> ComputeAk(CImg<> &imgIn)
{
    CImg<>
        integralImage(imgIn.width(), imgIn.height(), 1, 1, 0),
        Ak(imgIn.width(), imgIn.height(), 5, 1, 0);

    // Initialization using integral images
    cimg_forXY(integralImage, x, y)
    {
        float
            l1 = x == 0 ? 0 : integralImage(x - 1, y),
            l2 = y == 0 ? 0 : integralImage(x, y - 1),
            l3 = x == 0 || y == 0 ? 0 : integralImage(x - 1, y - 1);
        integralImage(x, y) = imgIn(x, y) + l1 + l2 - l3;
    }

    // Computing the Ak.
    int kp = 1;
    cimg_forZ(Ak, k)
    {
        kp *= 2;
        cimg_forXY(Ak, x, y)
            Ak(x, y, k) = IntegralMean(integralImage, x, y, kp);
    }
    return Ak;
}

/*
  Absolute difference computation
  Ak      : Image of local means
  Ekh,Ekv : Images of horizontal and vertical differences
            the k scale is encoded in z
*/
void ComputeE(CImg<> &Ak, CImg<> &Ekh, CImg<> &Ekv)
{
    int kp = 1;
    cimg_forZ(Ekh, k)
    {
        int k2 = kp;
        kp *= 2;
        cimg_forXY(Ekh, x, y)
        {
            int
                posx1 = x + k2,
                posx2 = x - k2,
                posy1 = y + k2,
                posy2 = y - k2;
            if (posx1 < Ak.width() && posx2 >= 0)
                Ekh(x, y, k) = cimg::abs(Ak(posx1, y, k) - Ak(posx2, y, k));
            if (posy1 < Ak.height() && posy2 >= 0)
                Ekv(x, y, k) = cimg::abs(Ak(x, posy1, k) - Ak(x, posy2, k));
        }
    }
}

/*
  Tamura’s coarseness.
  Ekh,Ekv : Images of horizontal and vertical differences
            the k scale is encoded in z
*/
float ComputeS(CImg<> &Ekh, CImg<> &Ekv)
{
    float sum = 0;
    cimg_forXY(Ekh, x, y)
    {
        float maxE = 0;
        int maxk = 0;
        cimg_forZ(Ekh, k)
            if (std::max(Ekh(x, y, k), Ekv(x, y, k)) > maxE)
            {
                maxE = std::max(Ekh(x, y, k), Ekv(x, y, k));
                maxk = k + 1;
            }
        sum += pow(2, maxk);
    }
    return sum / (Ekh.width() * Ekh.height());
}

/*
  Tamura’s coarseness.
  imgIn : Input image
*/
float Coarseness(CImg<> &imgIn)
{
    CImg<> Ak = ComputeAk(imgIn);
    CImg<>
        Ekh(imgIn.width(), imgIn.height(), 5, 1, 0),
        Ekv(imgIn.width(), imgIn.height(), 5, 1, 0);
    ComputeE(Ak, Ekh, Ekv);
    return ComputeS(Ekh, Ekv);
}

int main()
{
    CImg<> imgIn2("../images/farm.png");
    float coarseness_original = Coarseness(imgIn2);
    std::cout << "Coarseness: " << coarseness_original << std::endl;

    float coarseness_smooth = Coarseness(imgIn2.blur(5.0f));
    imgIn2.save_png("./results/farm_smooth.png");
    std::cout << "Coarseness (smooth): " << coarseness_smooth << std::endl;
}