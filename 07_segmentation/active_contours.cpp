/*
    Active contours (geodesic model) segmentation
*/

#define cimg_use_png
#include "CImg.h"

#include <string>
#include <iostream>
#include <algorithm>

using namespace cimg_library;

/*
    DrawLevelSet: Draw the zero level-set of the function psi
*/
CImg<> drawLevelSet(CImg<> &LevelSet)
{
    CImg<> imgOut(LevelSet.width(), LevelSet.height(), 1, 3);
    imgOut.fill(0);
    cimg_forXY(imgOut, x, y)
    {
        if (LevelSet(x, y) < 0)
            imgOut(x, y, 0) = 255;
        else
            imgOut(x, y, 2) = 255;
    }
    return imgOut;
}

/*
    InitLevelSet: Initialization of the LevelSet (psi) using the
    signed euclidean distance. The initial contour is a circle of
    center (x0,y0) and radius r.
    imgIn : Level-set function to be initialized
    x0,y0,r : Circle parameters to initialize
*/
void InitLevelSet(CImg<> &imgIn, int x0, int y0, int r)
{
    cimg_forXY(imgIn, x, y)
        imgIn(x, y) = std::sqrt(cimg::sqr(x - x0) + cimg::sqr(y - y0)) - r;
}

/*
    Propagate : Propagation algorithm of an implicit contour
    (geodesic model)
    imgIn :    Image to be segmented
    LevelSet : Level-set function (initialized)
*/
void Propagate(CImg<> &imgIn, CImg<> &LevelSet)
{
    float
        delta_t = 2.0f, // Temporal step
        alpha = 0.03f,  // Weighting of the propagation term
        beta = 1.0f,    // Weighting of the advection term
        ballon = 0.01f; // Balloon force (>0)
    int
        nbiter = 400, // Number of iterations
        exp_cont = 1; // Expansion (1) or contraction (-1) of F0

    // Computation of regularized gradients.
    CImgList<> GradImg = imgIn.get_gradient("xy", 4);

    // Function of the geodesic model.
    CImg<> f(imgIn.width(), imgIn.height());
    cimg_forXY(f, x, y)
    {
        float
            gx = GradImg(0, x, y),
            gy = GradImg(1, x, y);
        f(x, y) = exp_cont * (1. / (1 + std::sqrt(gx * gx + gy * gy)) + ballon);
    }

    // Computation of the gradient of f.
    CImgList<> Grad_f = f.get_gradient();

    // Iterative scheme of the propagation algorithm.
    for (int iter = 0; iter < nbiter; ++iter)
    {
        // Spatial gradient of psi.
        CImgList<>
            GradLS_minus = LevelSet.get_gradient("xy", -1),
            GradLS_plus = LevelSet.get_gradient("xy", 1);
        cimg_forXY(LevelSet, x, y)
        {
            float
                Dxm = GradLS_minus(0, x, y),
                Dxp = GradLS_plus(0, x, y),
                Dym = GradLS_minus(1, x, y),
                Dyp = GradLS_plus(1, x, y);
            // Propagation speed term: F0.
            float
                Nabla_plus = std::sqrt(cimg::sqr(std::max(Dxm, 0.0f)) +
                                       cimg::sqr(std::min(Dxp, 0.0f)) +
                                       cimg::sqr(std::max(Dym, 0.0f)) +
                                       cimg::sqr(std::min(Dyp, 0.0f))),
                Nabla_minus = std::sqrt(cimg::sqr(std::max(Dxp, 0.0f)) +
                                        cimg::sqr(std::min(Dxm, 0.0f)) +
                                        cimg::sqr(std::max(Dyp, 0.0f)) +
                                        cimg::sqr(std::min(Dym, 0.0f))),
                Fprop = -(std::max(f(x, y), 0.0f) * Nabla_plus + std::min(f(x, y), 0.0f) * Nabla_minus);
            // Advection speed term: Fadv.
            float
                u = Grad_f(0, x, y),
                v = Grad_f(1, x, y),
                Fadv = -(std::max(u, 0.0f) * Dxm + std::min(u, 0.0f) * Dxp +
                         std::max(v, 0.0f) * Dym + std::min(v, 0.0f) * Dyp);
            // Update of the level-set function.
            LevelSet(x, y) += delta_t * (alpha * Fprop + beta * Fadv);
        }

        // Normalization of the level-set function.
        if (!(iter % 20))
            LevelSet.distance_eikonal(10, 3);

        // Debug draw
        if (!(iter % 40))
            // (imgIn, drawLevelSet(LevelSet)).display(("Geodesic model segmentation (iteration " + std::to_string(iter) + ")").c_str());
            drawLevelSet(LevelSet).save_png(("./results/active_contours_" + std::to_string(iter) + ".png").c_str());
    }
}

int main()
{
    CImg<> imgIn("../images/coins.png"); // change this to test other images
    imgIn.norm().blur(0.75).threshold(imgIn.median() + 30).blur_median(3.0f).resize(256, 128);
    imgIn.get_normalize(0, 255).save_png("./results/active_contours_input.png");

    // Initialization of the level-set function.
    CImg<> LevelSet(imgIn.width(), imgIn.height());
    InitLevelSet(LevelSet, 192, 77, 10);

    // Propagation of the level-set function.
    Propagate(imgIn, LevelSet);

    // Display of the result.
    CImg<> imgOut = drawLevelSet(LevelSet);
    (imgIn, imgOut).display("Geodesic model segmentation");
    drawLevelSet(LevelSet).save_png("./results/active_contours_final.png");

    return 0;
}