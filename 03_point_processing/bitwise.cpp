#define cimg_use_png
#include "CImg.h"

using namespace cimg_library;

int main()
{
    // Create two images with specified values
    CImg<unsigned char> img1(200, 200, 1, 1, 0);
    CImg<unsigned char> img2(200, 200, 1, 1, 0);

    // Set the first few rows of img1 to 200
    for (int y = 0; y < 50; y++)
    {
        cimg_forX(img1, x)
        {
            img1(x, y) = 200;
        }
    }

    // Set the first few columns of img2 to 128
    for (int x = 0; x < 40; x++)
    {
        cimg_forY(img2, y)
        {
            img2(x, y) = 128;
        }
    }

    // Perform the bitwise AND operation
    CImg<unsigned char> img_and = img1 & img2;

    // Perform the bitwise OR operation
    CImg<unsigned char> img_or = img1 | img2;

    // Perform the bitwise XOR operation
    CImg<unsigned char> img_xor = img1 ^ img2;

    // Save the results
    img1.save("results/img1.png");
    img2.save("results/img2.png");
    img_and.save("results/and_image.png");
    img_or.save("results/or_image.png");
    img_xor.save("results/xor_image.png");

    return 0;
}
