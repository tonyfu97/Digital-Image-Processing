/*
    Phase correlation with Kalman filter.
*/

#define cimg_use_png
#include "CImg.h"

#include <cmath>
#include <string>
#include <iostream>

using namespace cimg_library;

std::string imageName = "shuffleboard";

CImg<> DrawRectangle(CImg<> &img, int x, int y, int width, int height)
{
    CImg<unsigned char> outputImage(img.width(), img.height(), 1, 3, 0);
    unsigned char color[] = {243, 102, 25};
    outputImage
    .draw_image(0, 0, 0, 0, img, 1)
    .draw_image(0, 0, 0, 1, img, 1)
    .draw_image(0, 0, 0, 2, img, 1)
    .draw_rectangle(x, y, x + width, y + height, color, 0.5f);
    return outputImage;
}

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

/**
 * Linear Kalman filter.
 *
 * @param I     List of images
 * @param pos   Initial position of the tracked object
 * @param size  Size of the window surrounding the object
 **/
void Kalman(CImgList<> &ImgList, CImg<int> &pos, CImg<int> &size)
{
    CImg<>
        prevState(1, 4),
        currState(1, 4);
    CImg<int> prevPos(pos), currPos(1, 2), estimPos(1, 2);
    prevState(0, 0) = pos(0);
    prevState(0, 1) = pos(1);
    prevState(0, 2) = 0;
    prevState(0, 3) = 0;
    CImg<> T = ImgList[0].get_crop(pos(0) - size(0), pos(1) - size(1),
                                   pos(0) + size(0), pos(1) + size(1));
    // Matrix D, constant speed model and Delta t = 1.
    CImg<> D = CImg<>::identity_matrix(4);
    D(2, 0) = D(3, 1) = 1;
    // Matrix M.
    CImg<> M(4, 2, 1, 1, 0);
    M(0, 0) = M(1, 1) = 1;
    CImg<> Mt = M.get_transpose();
    // Covariance of the model.
    CImg<> SigmaD = CImg<>::identity_matrix(4) * 5;
    SigmaD(2, 2) = SigmaD(3, 3) = 1;
    // Covariance of the measure.
    CImg<> SigmaM = CImg<>::identity_matrix(2) * 50;
    // A priori estimation of the error.
    CImg<> PkPrevious(4, 4, 1, 1, 0);
    // A posteriori estimation of the error.
    CImg<> PkCurrent(4, 4);
    for (int i = 1; i < ImgList.size(); ++i)
    {
        CImg<> currentSlice = ImgList[i-1].get_crop(prevPos(0), prevPos(1), prevPos(0) + size(0) - 1, prevPos(1) + size(1) - 1);
        CImg<> nextSlice = ImgList[i].get_crop(prevPos(0), prevPos(1), prevPos(0) + size(0) - 1, prevPos(1) + size(1) - 1);

        // Perform phase correlation
        int horizontalTranslation, verticalTranslation;
        PhaseCorrelation(currentSlice, nextSlice, horizontalTranslation, verticalTranslation);

        // Update the estimated position
        estimPos(0) = prevPos(0) - horizontalTranslation;
        estimPos(1) = prevPos(1) - verticalTranslation;

        // Prediction.
        currState = D * prevState;
        PkCurrent = SigmaD + D * PkPrevious * D.get_transpose();
        // Correction.
        CImg<> Kt = PkCurrent * Mt * ((M * PkCurrent * Mt + SigmaM).get_invert());
        currState = currState + Kt * (estimPos - M * currState);
        CImg<> I = CImg<>::identity_matrix(PkCurrent.width());
        PkCurrent = (I - Kt * M) * PkCurrent;
        // Update of the position.
        currPos(0) = (int)currState(0);
        currPos(1) = (int)currState(1);
        prevPos(0) = (int)currState(0);
        prevPos(1) = (int)currState(1);
        prevState = currState;
        PkPrevious = PkCurrent;

        CImg<> outputImg = DrawRectangle(ImgList[i], currPos(0), currPos(1), size(0), size(1));
        
        std::string fileName = "./results/kalman_" + imageName + "_" + std::to_string(i) + ".png";
        outputImg.normalize(0, 255).save_png(fileName.c_str());
    }
}

CImg<> ProcessImage(CImg<> &imgIn)
{
    imgIn.blur(0.25f).norm().normalize(0, 255);
    // imgIn.display();
    return imgIn;
}

int main()
{
    int totalFrames = 15;

    // Load image sequence
    CImgList<> imageSequence;
    CImg<> firstImage(("../docs/images/" + imageName + "_1.png").c_str());
    imageSequence.insert(ProcessImage(firstImage));
    for (int i = 0; i < totalFrames; ++i)
    {
        CImg<> nextImage(("../docs/images/" + imageName + "_" + std::to_string(i + 1) + ".png").c_str());
        imageSequence.insert(ProcessImage(nextImage));
    }

    // Initial ROI and tracking settings (for driveby)
    // int
    //     xCoordROI = 270,
    //     yCoordROI = 320,
    //     widthROI = 128,
    //     heightROI = 128;

    // Initial ROI and tracking settings (for shuffleboard)
    int
        xCoordROI = 365,
        yCoordROI = 65,
        widthROI = 128,
        heightROI = 128;

    CImg<int> pos(1, 2);
    pos(0) = xCoordROI;
    pos(1) = yCoordROI;
    CImg<int> size(1, 2);
    size(0) = widthROI;
    size(1) = heightROI;

    Kalman(imageSequence, pos, size);

    return 0;
}