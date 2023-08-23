/*
  Corner detector using Harris and Stephens algorithm.
  imgIn : Input image
  k     : Sensitivity parameter
  n     : Number of points to detect
  sigma : Variance of the Gaussian window
*/

#define cimg_use_png
#include "CImg.h"

using namespace cimg_library;

CImg<> get_gaussian(int size, float sigma)
{
    CImg<> kernel(size, size);
    int halfSize = size / 2;
    float sum = 0.0f;
    for (int x = -halfSize; x <= halfSize; x++)
    {
        for (int y = -halfSize; y <= halfSize; y++)
        {
            float value = exp(-0.5f * (x * x + y * y) / (sigma * sigma)) / (2 * M_PI * sigma * sigma);
            kernel(x + halfSize, y + halfSize) = value;
            sum += value;
        }
    }
    kernel /= sum; // Normalize the kernel
    return kernel;
}

CImg<> HarrisDetector(CImg<> &imgIn, float k, int n, float sigma)
{
    CImg<> harris(imgIn);
    CImgList<> gradXY = imgIn.get_gradient();
    const float eps = 1e-5f;

    // Windowing.
    CImg<> G = get_gaussian(7, sigma);

    // Structure tensor.
    CImg<>
        Ixx = gradXY[0].get_mul(gradXY[0]).get_convolve(G),
        Iyy = gradXY[1].get_mul(gradXY[1]).get_convolve(G),
        Ixy = gradXY[0].get_mul(gradXY[1]).get_convolve(G);

    // R function.
    CImg<>
        det = Ixx.get_mul(Iyy) - Ixy.get_sqr(),
        trace = Ixx + Iyy,
        R = det - k * trace.get_sqr();

    // Local maxima of R.
    CImgList<> imgGradR = R.get_gradient();
    CImg_3x3(I, float);
    CImg<> harrisValues(imgIn.width() * imgIn.height(), 1, 1, 1, 0);
    CImg<int>
        harrisXY(imgIn.width() * imgIn.height(), 2, 1, 1, 0),
        perm(imgIn.width() * imgIn.height(), 1, 1, 1, 0);
    int nbHarris = 0;
    cimg_for3x3(R, x, y, 0, 0, I, float)
    {
        if (imgGradR[0](x, y) < eps && imgGradR[1](x, y) < eps)
        {
            float
                befx = Ipc - Icc,
                befy = Icp - Icc,
                afty = Icn - Icc,
                aftx = Inc - Icc;
            if (befx < 0 && befy < 0 && aftx < 0 && afty < 0)
            {
                harrisValues(nbHarris) = R(x, y);
                harrisXY(nbHarris, 0) = x;
                harrisXY(nbHarris++, 1) = y;
            }
        }
    }
    // Sorting.
    harrisValues.sort(perm, false);

    // Display of the points of interest.
    unsigned char red[] = {200, 0, 0};
    int line_length = 10;   // Increase the size of the cross
    int line_thickness = 2; // Increase the line thickness

    for (int i = 0; i < n; ++i)
    {
        int pos = perm(i),
            posx = harrisXY(pos, 0),
            posy = harrisXY(pos, 1);

        harris.draw_line(posx - line_length, posy, posx + line_length, posy, red, line_thickness);
        harris.draw_line(posx, posy - line_length, posx, posy + line_length, red, line_thickness);
    }

    return harris;
}

int main()
{
    CImg<unsigned char> img("../images/lighthouse.png");

    // Convert to grayscale
    CImg<> lum = img.resize(512, 256); //.get_norm().blur(0.75f)

    // Harris detector
    CImg<> imgOut = HarrisDetector(lum, 0.04f, 50, 2.0f);
    imgOut.normalize(0, 255).save("./results/lighthouse_harris.png");

    return 0;
}