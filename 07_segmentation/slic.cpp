/*
    Simple Linear Iterative Clustering (SLIC) superpixels
*/

#define cimg_use_png
#include "CImg.h"

#include <cmath>
#include <string>
#include <iostream>

using namespace cimg_library;

// Pixel structure
struct Pixel
{
    float x, y, L, a, b;
};

// Distance between two pixels, serving as the objective function.
float D(const Pixel &p1, const Pixel &p2, float S, float m)
{
    return std::pow(p1.L - p2.L, 2) + std::pow(p1.a - p2.a, 2) +
           std::pow(p1.b - p2.b, 2) + m * m / (S * S) * (std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}

// Assign a centroid to each image pixel.
CImg<> get_labels(CImg<> &lab, CImg<> &centroids, float S, float m)
{
    CImg<> labels(lab.width(), lab.height(), 1, 2, 1e20);

    // Test neighbor pixels of each centroid, in a neighborhood of size 2Sx2S.
    cimg_forX(centroids, k)
    {
        Pixel centroid = {centroids(k, 0),
                          centroids(k, 1),
                          centroids(k, 2),
                          centroids(k, 3),
                          centroids(k, 4)};
        int x0 = std::max((int)(centroid.x - S), 0);
        int y0 = std::max((int)(centroid.y - S), 0);
        int x1 = std::min((int)(centroid.x + S - 1), lab.width() - 1);
        int y1 = std::min((int)(centroid.y + S - 1), lab.height() - 1);

        cimg_for_inXY(lab, x0, y0, x1, y1, x, y)
        {
            Pixel currentPixel = {static_cast<float>(x),
                                  static_cast<float>(y),
                                  lab(x, y, 0),
                                  lab(x, y, 1),
                                  lab(x, y, 2)};
            float dist = D(currentPixel, centroid, S, m);

            // Update the label if the distance is smaller.
            if (dist < labels(x, y, 1))
            {
                labels(x, y, 0) = k;
                labels(x, y, 1) = dist;
            }
        }
    }

    // Label remaining pixels by testing them against all centroids.
    cimg_forXY(labels, x, y) if (labels(x, y) >= centroids.width())
    {
        Pixel currentPixel = {static_cast<float>(x),
                              static_cast<float>(y),
                              lab(x, y, 0),
                              lab(x, y, 1),
                              lab(x, y, 2)};
        float distmin = 1e20;
        int kmin = 0;

        // Test against all centroids.
        cimg_forX(centroids, k)
        {
            Pixel centroid = {centroids(k, 0),
                              centroids(k, 1),
                              centroids(k, 2),
                              centroids(k, 3),
                              centroids(k, 4)};
            float dist = D(currentPixel, centroid, S, m);

            if (dist < distmin)
            {
                distmin = dist;
                kmin = k;
            }
        }
        labels(x, y, 0) = kmin;
        labels(x, y, 1) = distmin;
    }

    return labels;
}

// Initialize the centroids as the position of the minimal gradient
CImg<> intialize_centroids(CImg<> &img, float S)
{
    // Initialize the centroids as a grid, with cell size SxS.
    CImg<> centroids(cimg::round(img.width() / S),
                     cimg::round(img.height() / S), 1, 5);
    int S1 = S / 2;
    int S2 = S - 1 - S1;

    cimg_forXY(centroids, x, y)
    {
        int
            xc = x * S + S1,
            yc = y * S + S1,
            x0 = std::max(xc - S1, 0),
            y0 = std::max(yc - S1, 0),
            x1 = std::min(xc + S2, img.width() - 1),
            y1 = std::min(yc + S2, img.height() - 1);
        // Retrieve the position of the point with the minimal gradient
        // in a neighborhood of size SxS.
        CImg<> st = img.get_crop(x0, y0, x1, y1).get_stats();
        centroids(x, y, 0) = x0 + st[4];     // x0 + st[4] = x0 + st.min_x
        centroids(x, y, 1) = y0 + st[5];     // y0 + st[5] = y0 + st.min_y
        centroids(x, y, 2) = img(xc, yc, 0); // L
        centroids(x, y, 3) = img(xc, yc, 1); // a
        centroids(x, y, 4) = img(xc, yc, 2); // b
    }
    // Unroll as an image of size Nx1x1x5.
    centroids.resize(centroids.width() * centroids.height(), 1, 1, 5, -1);
    return centroids;
}

// Recomputes the average positions and colors of each centroid.
CImg<> recompute_centroids(const CImg<> &img, const CImg<> &lab, const CImg<> &labels, const CImg<> &centroids)
{
    CImg<> next_centroids = centroids.get_fill(0);
    CImg<> accu(centroids.width(), 1, 1, 1, 0);

    cimg_forXY(img, x, y)
    {
        int k = (int)labels(x, y);
        next_centroids(k, 0) += x;
        next_centroids(k, 1) += y;
        next_centroids(k, 2) += lab(x, y, 0);
        next_centroids(k, 3) += lab(x, y, 1);
        next_centroids(k, 4) += lab(x, y, 2);
        ++accu[k];
    }
    accu.max(1e-8f);
    next_centroids.div(accu);
    return next_centroids;
}

// Computes the residual error using the L1 norm.
float compute_residual_error(const CImg<> &next_centroids, const CImg<> &centroids)
{
    return (next_centroids - centroids).norm(1).sum() / centroids.width();
}

// Draw the superpixel boundaries.
CImg<> draw_boundaries(const CImg<> &img, const CImg<> &labels, const CImg<> &centroids)
{
    CImg<unsigned char> visu = labels.get_map(centroids.get_channels(2, 4)).LabtoRGB();

    CImg<> N(9);
    cimg_for3x3(labels, x, y, 0, 0, N, float) if (N[4] != N[1] || N[4] != N[3])
        visu.fillC(x, y, 0, 0, 0, 0);

    unsigned char red[] = {255, 0, 0};
    cimg_forX(centroids, k)
        visu.draw_circle((int)centroids(k, 0), (int)centroids(k, 1), 2, red, 0.5f);

    return visu;
}

// Simple Linear Iterative Clustering (SLIC) superpixels
CImg<> SLIC(CImg<> &img, float S, float m, int max_iter)
{
    // remove alpha channel if any
    if (img.spectrum() == 4)
        img.channels(0, 2);

    CImg<> lab = img.get_RGBtoLab(); // Conversion RGB -> Lab
    CImgList<> grad = lab.get_gradient("xy");
    CImg<> grad_norm = (grad > 'c').norm(); // Gradient norm

    // Initialize the centroids as the position of the minimal gradient
    CImg<> centroids = intialize_centroids(grad_norm, S);

    float residualError = 0;
    do
    {
        // Assign a centroid to each image pixel.
        CImg<> labels = get_labels(lab, centroids, S, m);

        // Recompute the average positions and colors of each centroid.
        CImg<> next_centroids = recompute_centroids(img, lab, labels, centroids);

        residualError = compute_residual_error(next_centroids, centroids);
        centroids.swap(next_centroids);
    } while (residualError > 0.25f);

    CImg<> labels = get_labels(lab, centroids, S, m).channel(0);

    return draw_boundaries(img, labels, centroids);
}

int main()
{
    CImg<> img("../images/car.png");
    img.blur(2.5f).normalize(0, 255);

    const int S = 40; // Superpixel size
    const int m = 10; // Compactness factor

    CImg<> visu = SLIC(img, S, m, 10);
    visu.save("./results/slic.png");
}