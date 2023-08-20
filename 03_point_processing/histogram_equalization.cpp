/*
  Histogram equalization
  imgIn : Input image
  nb    : Number of bins
*/
#include <string>

#define cimg_use_png
#include "CImg.h"

using namespace cimg_library;

CImg<> equalizeHisto(CImg<> &imgIn, unsigned int nb)
{
    CImg<> imgOut(imgIn);                   // Create a copy of the input image for the output
    float vmin, vmax = imgIn.max_min(vmin); // Find the minimum and maximum pixel values
    int size = imgIn.size();                // Get the total number of pixels in the image
    int vdiff = vmax - vmin;                // Compute the difference between max and min values

    CImg<> hist = imgIn.get_histogram(nb, vmin, vmax); // Calculate the histogram with nb bins
    long int cumul = 0;

    // Compute the cumulative histogram
    cimg_forX(hist, pos)
    {
        cumul += hist[pos];
        hist[pos] = cumul;
    }

    if (cumul == 0) // Check for a special case where the image has no non-zero pixels
        cumul = 1;  // Avoid division by zero later in the code

    // Equalize the image by adjusting pixel values according to the cumulative histogram
    cimg_foroff(imgIn, off) // Iterate through all offsets (positions) in the input image
    {
        int pos = (int)((imgIn[off] - vmin) * (nb - 1) / vdiff);
        if (pos >= 0 && pos < (int)nb)
            imgOut[off] = vmin + vdiff * hist[pos] / size;
    }

    return imgOut;
}

int main(int argc, char **argv)
{
    // Read image filename from command line or use default
    cimg_usage("Histogram equalization");
    std::string filename = cimg_option("-i", "./results/lighthouse_cube.png", "Color image");
    std::string file_basename = filename.substr(9, filename.size() - 13);

    // Read image
    CImg<> img(filename.c_str());
    CImg<> lum = img.get_norm().blur(0.75f).normalize(0, 255);

    // Calculate and display the original histogram
    CImg<> original_hist = lum.get_histogram(256);
    double original_max = original_hist.max();
    CImgDisplay original_disp;
    original_hist.display_graph(original_disp, 3, 2, "Original Histogram", 0, 0, "Intensity", 0, original_max);

    // Equalize histogram
    CImg<> eqImg = equalizeHisto(lum, 256);

    // Calculate and display the equalized histogram
    CImg<> equalized_hist = eqImg.get_histogram(256);
    double equalized_max = equalized_hist.max();
    CImgDisplay equalized_disp;
    equalized_hist.display_graph(equalized_disp, 3, 2, "Equalized Histogram", 0, 0, "Intensity", 0, equalized_max);

    eqImg.normalize(0, 255).save(("results/" + file_basename + "_equalized.png").c_str());

    // Keep the display window open
    while (!original_disp.is_closed() && !equalized_disp.is_closed()) {
        original_disp.wait();
        equalized_disp.wait();
    }

    return 0;
}
