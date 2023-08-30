/*
    Phase correlation for sparse optical flow.
*/

#define cimg_use_png
#include "CImg.h"

#include <cmath>
#include <string>
#include <iostream>

using namespace cimg_library;

/**
 * Performs image registration via phase correlation.
 *
 * @param sourceImg Source image.
 * @param targetImg Target image.
 * @param outHorizTranslation Output horizontal translation.
 * @param outVertTranslation Output vertical translation.
 */
void PhaseCorrelation(CImg<> &sourceImg,
                      CImg<> &targetImg,
                      int &outHorizTranslation,
                      int &outVertTranslation)
{
    const float EPSILON = 1.0e-8f;

    // Compute Fourier Transform of both images
    CImgList<> sourceFFT = sourceImg.get_FFT("xy");
    CImgList<> targetFFT = targetImg.get_FFT("xy");

    // Compute Cross-Power Spectrum
    CImg<> realCrossPower = sourceFFT(0).get_mul(targetFFT(0)) + sourceFFT(1).get_mul(targetFFT(1));
    CImg<> imagCrossPower = targetFFT(0).get_mul(sourceFFT(1)) - sourceFFT(0).get_mul(targetFFT(1));
    CImg<> denominator = (realCrossPower.get_mul(realCrossPower) + imagCrossPower.get_mul(imagCrossPower)).get_sqrt();

    realCrossPower.div(denominator + EPSILON);
    imagCrossPower.div(denominator + EPSILON);

    // Compute Inverse FFT of Phase Correlation
    CImg<>::FFT(realCrossPower, imagCrossPower, true);
    CImg<> phaseCorrelation = (realCrossPower.get_mul(realCrossPower) + imagCrossPower.get_mul(imagCrossPower)).get_sqrt();

    // Find the maximum correlation point
    float maxCorrelation = 0;
    int width = sourceImg.width();
    int height = sourceImg.height();

    cimg_forXY(phaseCorrelation, x, y)
    {
        if (phaseCorrelation(x, y) > maxCorrelation)
        {
            maxCorrelation = phaseCorrelation(x, y);
            outHorizTranslation = -((x - 1 + (width / 4)) % (width / 2) - (width / 4) + 1);
            outVertTranslation = -((y - 1 + (height / 4)) % (height / 2) - (height / 4) + 1);
        }
    }

    std::cout << "Max correlation: " << maxCorrelation << ", Horizontal translation: " << outHorizTranslation << ", Vertical translation: " << outVertTranslation << std::endl;
}

CImg<> ProcessImage(CImg<> &imgIn)
{
    imgIn.blur(0.25f).norm().normalize(0, 255);
    // imgIn.display();
    return imgIn;
}

int main(int argc, const char **argv)
{
    // Initial ROI and tracking settings
    int
        xCoordROI = 365,
        yCoordROI = 65,
        widthROI = 128,
        heightROI = 128;
    bool saveResults = true;

    std::string imageName = "shuffleboard";
    int totalFrames = 15;

    // Load image sequence
    CImg<> imageSequence(("../docs/images/" + imageName + "_1.png").c_str());
    imageSequence = ProcessImage(imageSequence);
    for (int i = 0; i < totalFrames; ++i)
    {
        CImg<> nextImage(("../docs/images/" + imageName + "_" + std::to_string(i + 1) + ".png").c_str());
        imageSequence.append(ProcessImage(nextImage), 'z');
    }

    int horizontalTranslation, verticalTranslation;
    int xCoord = xCoordROI, yCoord = yCoordROI;

    // Loop through frames and perform tracking
    for (int frame = 0; frame < imageSequence.depth() - 1; ++frame)
    {
        CImg<> currentSlice = imageSequence.get_slice(frame).crop(xCoord, yCoord, xCoord + widthROI - 1, yCoord + heightROI - 1);
        CImg<> nextSlice = imageSequence.get_slice(frame + 1).crop(xCoord, yCoord, xCoord + widthROI - 1, yCoord + heightROI - 1);

        // Perform phase correlation for motion estimation
        PhaseCorrelation(currentSlice,
                         nextSlice,
                         horizontalTranslation,
                         verticalTranslation);

        // Update object position based on motion
        xCoord -= horizontalTranslation;
        yCoord -= verticalTranslation;

        // Optionally save tracking results
        if (saveResults)
        {
            CImg<unsigned char> outputImage(imageSequence.width(), imageSequence.height(), 1, 3, 0);
            unsigned char color[] = {243, 102, 25};
            outputImage.
            draw_image(0, 0, 0, 0, imageSequence.get_slice(frame + 1), 1).
            draw_image(0, 0, 0, 1, imageSequence.get_slice(frame + 1), 1).
            draw_image(0, 0, 0, 2, imageSequence.get_slice(frame + 1), 1).
            draw_rectangle(xCoord, yCoord, xCoord + widthROI - 1, yCoord + heightROI - 1, color, 0.5f);

            std::string fileName = "./results/phase_correlation_" + imageName + "_" + std::to_string(frame + 1) + ".png";
            outputImage.normalize(0, 255).save_png(fileName.c_str());
        }
    }
    return 0;
}
