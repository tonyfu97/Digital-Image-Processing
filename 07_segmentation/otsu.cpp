/*
    Otsu's algorithm (histogram-based segmentation)
*/

#define cimg_use_png
#include "CImg.h"

#include <string>
#include <iostream>
#include <algorithm>

using namespace cimg_library;

/*
  Otsu algorithm
  imgIn     : input image
  nb_levels : number of bins
*/
float Otsu(CImg<> &imgIn, int nb_levels)
{
    // Initialize maximum between-class variance and other variables
    float max_variance = 0;
    long num_background_pixels = 0, num_foreground_pixels;
    int optimal_threshold = -1;

    // Calculate histogram of the input image
    CImg<> histogram = imgIn.get_histogram(nb_levels);

    // Loop through all intensity levels
    cimg_forX(histogram, i)
    {
        if (i < nb_levels - 1)
        {
            // Update the count of background pixels
            num_background_pixels += histogram[i];
            // Calculate the count of foreground pixels
            num_foreground_pixels = imgIn.size() - num_background_pixels;

            float background_mean = 0, foreground_mean = 0;

            // Calculate mean intensity of background class
            for (int j = 0; j <= i; ++j)
                background_mean += j * histogram[j];
            background_mean /= num_background_pixels;

            // Calculate mean intensity of foreground class
            for (int j = i + 1; j < nb_levels; ++j)
                foreground_mean += j * histogram[j];
            foreground_mean /= num_foreground_pixels;

            // Only proceed if both classes have pixels
            if (num_background_pixels * num_foreground_pixels > 0)
            {
                // Calculate the between-class variance
                float variance = num_background_pixels * num_foreground_pixels * cimg::sqr(background_mean - foreground_mean);

                // Update the optimal threshold if current variance is greater
                if (variance > max_variance)
                {
                    max_variance = variance;
                    optimal_threshold = i;
                }
            }
        }
    }

    return (float)optimal_threshold;
}

int main(int argc, char *argv[]) {
    CImg<> img("../images/bay.png");
    img.norm().blur(0.75f).resize(512, 256);
    img.normalize(0, 255).save_png("./results/otsu_input.png");
    img.get_histogram(256).display_graph("Histogram of the input image", 3);

    int nb_levels = 256;
    float optimal_threshold = Otsu(img, nb_levels);
    std::cout << "Optimal threshold: " << optimal_threshold << std::endl;

    // Apply the threshold to create a binary image
    CImg<float> img_segmented = img.get_threshold(optimal_threshold);
    img_segmented.normalize(0, 255).save_png("./results/otsu_output.png");

    return 0;
}
