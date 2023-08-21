/*
  Skeletonization by iterative removal of pixels.
  imgIn : Input image
*/

#define cimg_use_png
#include "CImg.h"

using namespace cimg_library;

int IterSkeleton(CImg<> &imgIn)
{
    // Pixel tag image for removal.
    CImg<unsigned char> D(imgIn.width(), imgIn.height(), 1, 1, 0);

    // Neighborhood.
    CImg_3x3(N, unsigned char);

    // Pass 1.
    int n1 = 0;
    cimg_for3x3(imgIn, x, y, 0, 0, N, unsigned char)
    {
        if (imgIn(x, y))
        {
            unsigned char
                B = Npp + Ncp + Nnp + Npc + Nnc + Npn + Ncn + Nnn,
                C = Nnc * (Nnc - Nnp) + Nnp * (Nnp - Ncp) +
                    Ncp * (Ncp - Npp) + Npp * (Npp - Npc) +
                    Npc * (Npc - Npn) + Npn * (Npn - Ncn) +
                    Ncn * (Ncn - Nnn) + Nnn * (Nnn - Nnc);
            bool R1 = B >= 2 && B <= 6 && C == 1 && Ncn * Nnc * Ncp == 0 && Npc * Ncn * Nnc == 0;
            if (R1)
            {
                // Tag (x,y).
                D(x, y) = 1;
                ++n1;
            }
        }
    }

    // Removing tagged pixels.
    cimg_forXY(imgIn, x, y)
        imgIn(x, y) -= (n1 > 0) * D(x, y);

    // Pass 2.
    int n2 = 0;
    D.fill(0);
    cimg_for3x3(imgIn, x, y, 0, 0, N, unsigned char)
    {
        if (imgIn(x, y))
        {
            unsigned char
                B = Npp + Ncp + Nnp + Npc + Nnc + Npn + Ncn + Nnn,
                C = Nnc * (Nnc - Nnp) + Nnp * (Nnp - Ncp) +
                    Ncp * (Ncp - Npp) + Npp * (Npp - Npc) +
                    Npc * (Npc - Npn) + Npn * (Npn - Ncn) +
                    Ncn * (Ncn - Nnn) + Nnn * (Nnn - Nnc);
            bool R2 = B >= 2 && B <= 6 && C == 1 && Nnc * Ncp * Npc == 0 && Ncp * Npc * Ncn == 0;
            if (R2)
            {
                // Tag (x,y)
                D(x, y) = 1;
                ++n2;
            }
        }
    }

    // Removing tagged pixels.
    cimg_forXY(imgIn, x, y)
        imgIn(x, y) -= (n2 > 0) * D(x, y);

    // Number of removed pixels.
    return n1 + n2;
}

int main()
{
    CImg<unsigned char> img("../images/words_and_shapes.png");

    // Convert to grayscale
    CImg<> lum = img.get_norm().blur(0.75f);

    // Thresholding
    lum.threshold(lum.mean() + 30).normalize(0, 1);

    lum.get_normalize(0, 255).save("./results/words_and_shapes_threshold.png");

    // Skeletonization.
    int num_removed;
    do
    {
        num_removed = IterSkeleton(lum);
    } while (num_removed > 0);

    lum.normalize(0, 255).save_png("./results/words_and_shapes_skeleton.png");

    return 0;
}