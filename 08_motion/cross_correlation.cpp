/*
    Spatial correlation for sparse optical flow.
*/

#define cimg_use_png
#include "CImg.h"

#include <cmath>
#include <string>
#include <iostream>

using namespace cimg_library;

struct Position
{
    int x;
    int y;
};

struct Size
{
    int width;
    int height;
};

struct Area
{
    int xRange;
    int yRange;
};

CImg<> DrawRectangle(CImg<> &image, Position position,
                     Size size,
                     unsigned char *color,
                     int thickness = 1,
                     unsigned int pattern = ~0U)
{
    CImg<> image_copy(image);
    image_copy.draw_rectangle(
        position.x - size.width,
        position.y - size.height,
        position.x + size.width,
        position.y + size.height,
        color, thickness, pattern);
    return image_copy;
}

// Track an object in an image sequence using cross-correlation
CImg<> TrackObjectByCrossCorrelation(CImgList<> &imageSequence,
                                     Position initialPosition,
                                     Size templateSize,
                                     Area searchArea)
{
    // Previous and current object positions
    Position
        prevPosition(initialPosition),
        currPosition;

    // Create normalized template around the initial position
    CImg<> templateImage = imageSequence[0].get_crop(
        initialPosition.x - templateSize.width,
        initialPosition.y - templateSize.height,
        initialPosition.x + templateSize.width,
        initialPosition.y + templateSize.height);
    int templateWidth = templateImage.width();
    int templateHeight = templateImage.height();
    templateImage -= templateImage.sum() / (templateWidth * templateHeight);

    // Variables for storing the best match
    float maxCorrelation = -1, currentCorrelation;
    float templateNorm = templateImage.magnitude();

    // Search for the object within the searchArea in the next image
    for (int x = prevPosition.x - searchArea.xRange; x <= prevPosition.x + searchArea.xRange; ++x)
    {
        for (int y = prevPosition.y - searchArea.yRange; y <= prevPosition.y + searchArea.yRange; ++y)
        {
            // Extract patch from the next image
            CImg<> patch = imageSequence[1].get_crop(
                x - templateWidth / 2,
                y - templateHeight / 2,
                x + templateWidth / 2,
                y + templateHeight / 2);
            patch -= patch.sum() / (patch.width() * patch.height());

            // Compute correlation with the template
            currentCorrelation = patch.dot(templateImage) / (patch.magnitude() * templateNorm);

            // Update the position if this patch is a better match
            if (currentCorrelation > maxCorrelation)
            {
                maxCorrelation = currentCorrelation;
                currPosition.x = x;
                currPosition.y = y;
            }
        }
    }

    // Draw rectangle around the new object position in the next image
    unsigned char red[3] = {255, 0, 0};
    CImg<> outputImage = DrawRectangle(imageSequence[1], currPosition, templateSize, red);

    return outputImage;
}

int main()
{
    std::string image_name = "shuffleboard";
    CImg<>
        img1(("../docs/images/" + image_name + "_1.png").c_str()),
        img2(("../docs/images/" + image_name + "_2.png").c_str());

    img1.normalize(0, 255).blur(1.0f);
    img2.normalize(0, 255).blur(1.0f).resize(img1);

    // Remove alpha channel
    if (img1.spectrum() == 4)
        img1.channels(0, 2);
    if (img2.spectrum() == 4)
        img2.channels(0, 2);

    // Put images in a list
    CImgList<> imageSequence(img1, img2);

    // Initial position, template size and search area
    Position initialPosition{172, 167};
    Size templateSize{30, 30};
    Area searchArea{60, 60};

    // Draw and save the initial image
    unsigned char red[3] = {255, 0, 0};
    CImg<> inputImageBoxed = DrawRectangle(imageSequence[0], initialPosition, templateSize, red);
    // inputImageBoxed.normalize(0, 255).display();
    inputImageBoxed.normalize(0, 255).save_png(("./results/cross_correlation_" + image_name + "_input.png").c_str());

    CImg<> outputImage = TrackObjectByCrossCorrelation(imageSequence, initialPosition, templateSize, searchArea);
    outputImage.normalize(0, 255).save_png(("./results/cross_correlation_" + image_name + "_output.png").c_str());

    return 0;
}