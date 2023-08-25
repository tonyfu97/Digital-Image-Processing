#define cimg_use_png
#include "CImg.h"

#include <string>

using namespace cimg_library;

/*
  Accumulator for circle detection.
  ImgIn : Input image
*/
CImg<> Hough_Circle(CImg<> &ImgIn)
{
    // Define the accumulator
    int Rmin = 15; // Minimum radius
    int Rmax = 35; // Maximum radius
    CImg<>
        Acc(ImgIn.width(), ImgIn.height(), Rmax - Rmin, 1, 0),
        imgOut(ImgIn.width(), ImgIn.height(), 1, 3, 0);

    // Gradient
    CImgList<> imgGrad = ImgIn.get_gradient();
    imgGrad[0].blur(2.0f).normalize(0, 255);
    imgGrad[1].blur(2.0f).normalize(0, 255);

    // Hough space
    cimg_forXY(ImgIn, x, y)
    {
        float
            gx = imgGrad(0, x, y),
            gy = imgGrad(1, x, y),
            norm = std::sqrt(gx * gx + gy * gy);
        if (norm > 180.0f)
        {
            cimg_forZ(Acc, r)
            {
                // Center in the direction of the gradient
                int
                    xc = (int)(x + (r + Rmin) * gx / norm),
                    yc = (int)(y + (r + Rmin) * gy / norm);
                // Voting scheme
                if (xc >= 0 && xc < Acc.width() && yc >= 0 && yc < Acc.height())
                    Acc(xc, yc, r) += norm;
                // Center in the opposite direction of the gradient.
                xc = (int)(x - (r + Rmin) * gx / norm);
                yc = (int)(y - (r + Rmin) * gy / norm);
                if (xc >= 0 && xc < Acc.width() && yc >= 0 && yc < Acc.height())
                    Acc(xc, yc, r) += norm;
            }
        }
    }

    // Smoothing the accumulator
    Acc.blur(1.0f).normalize(0, 255);

    // Circle display.
    unsigned char col1[3] = {255, 255, 0};
    cimg_forXYZ(Acc, x, y, z)
    {
        if (Acc(x, y, z) > 190.0f)
        {
            int radius = z + Rmin;
            imgOut.draw_circle(x, y, radius, col1, 1.0f);
        }
    }
    return imgOut;
}

int main()
{
    CImg<> img("../images/coins.png");
    img.norm().blur(0.75f).threshold(img.median()+30).blur_median(5).resize(256, 128);
    img.get_normalize(0, 255).save("./results/coins_median.png");

    // Hough transform
    CImg<> imgOut = Hough_Circle(img);

    // Save result
    imgOut.normalize(0, 255).save("./results/coins_hought_circle.png");

    return 0;
}
