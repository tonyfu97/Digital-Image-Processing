/*
    Principal Component Analysis (PCA)
*/

#define cimg_use_png
#include "CImg.h"

#include <cmath>
#include <string>
#include <iostream>

using namespace cimg_library;

std::string imgName = "cat";

CImg<> PCA(CImg<> &imgIn)
{
    int p = imgIn.spectrum();
    CImg<> mean(p, 1), inv_std_dev(p, 1);
    cimg_forC(imgIn, c)
    {
        mean(c) = imgIn.get_channel(c).mean();
        inv_std_dev(c) = 1. / std::sqrt(imgIn.get_channel(c).variance());
    }

    // Data matrix.
    CImg<> X(p, imgIn.width() * imgIn.height());
    cimg_forXYC(imgIn, x, y, c)
        X(c, x + y * imgIn.width()) = imgIn(x, y, 0, c);

    // Centered/standardized data.
    CImg<> ones(1, imgIn.width() * imgIn.height());
    inv_std_dev.diagonal();
    CImg<> Z = (X - ones * mean) * inv_std_dev;

    // Correlation matrix (up to p).
    CImg<> R = Z.get_transpose() * Z;

    // Eigenvalues and eigenvectors.
    CImgList<> eig = R.get_symmetric_eigen();
    CImg<> &val = eig[0],
           &vec = eig[1];

    // Calculate the number of principal components needed to capture 90% of the variance
    float s = 0.9 * val.sum(),
          inertia = val[0];
    int nb_pca = 1;
    while (inertia < s)
        inertia += val[nb_pca++];

    // Use only top nb_pca principal components.
    CImg<> reducedVec = vec.get_columns(0, 2); // why the iamge is still black and white when I use 2??
    CImg<> Xpca = X * reducedVec;

    // Reconstruct image.
    CImg<> imgOut(imgIn.width(), imgIn.height(), 1, nb_pca);
    cimg_forXYC(imgOut, x, y, c)
        imgOut(x, y, 0, c) = Xpca(c, x + y * imgIn.width());

    // For fun: Reconstruct using only one principal component at a time.
    CImg<> imgOut1Vec(imgIn.width(), imgIn.height(), 1, 3);
    for (int i = 0; i < 3; ++i)
    {
        reducedVec = vec.get_column(i);
        Xpca = X * reducedVec;
        cimg_forXYC(imgOut1Vec, x, y, c)
            imgOut1Vec(x, y, 0, c) = Xpca(0, x + y * imgIn.width());
        imgOut1Vec.normalize(0, 255).save_png(("./results/pca_" + std::to_string(i) + "_" + imgName + ".png").c_str());
    }

    return imgOut;
}

int main()
{
    CImg<> imgIn(("../images/" + imgName + ".png").c_str());
    // remove alpha channel
    if (imgIn.spectrum() == 4)
        imgIn = imgIn.get_channels(0, 2);

    CImg<> imgOut = PCA(imgIn);
    imgOut.normalize(0, 255).save_png(("./results/pca_" + imgName + ".png").c_str());

    return 0;
}
