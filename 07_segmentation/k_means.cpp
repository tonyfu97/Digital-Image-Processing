/*
    K-means segmentation
*/

#define cimg_use_png
#include "CImg.h"

#include <string>
#include <iostream>

using namespace cimg_library;

/*
    Features computation.
    imgIn : Input image (x, y, 1, 1, 1, 1)
*/
CImg<> ComputeFeatures(CImg<> &imgIn)
{
    CImg<> features(imgIn.width(), imgIn.height(), 2);
    // For each pixel, mean and variance in a 5x5 neighborhood.
    CImg<> N(5, 5);
    cimg_for5x5(imgIn, x, y, 0, 0, N, float)
    {
        features(x, y, 0) = N.mean();
        features(x, y, 1) = N.variance();
    }
    // Normalization
    features.get_shared_slice(0).normalize(0, 255);
    features.get_shared_slice(1).normalize(0, 255);
    return features;
}

/*
    Squared Euclidean distance between two vectors.
    classCenters : Class centers (ncl, dim)
    featureData  : Feature vectors (x, y, dim)
    x, y         : Pixel coordinates
*/
float d2(CImg<> &g_i, CImg<> &data, int x, int y)
{
    float d = 0;
    cimg_forX(g_i, dim)
        d += cimg::sqr(data(x, y, dim) - g_i(dim));
    return d;
}

/*
    Assigment of points to classes
    featureData  : Feature vectors (x, y, dim)
    classCenters : Class centers (ncl, dim)
    label        : Classes of the points (x, y)
*/
void AssignToNearestClass(CImg<> &featureData, CImgList<> &classCenters, CImg<unsigned int> &labels)
{
    float distance, minDistance;
    unsigned int nearestClass;

    cimg_forXY(featureData, x, y)
    {
        // Find the closest class center.
        nearestClass = 0;
        minDistance = d2(classCenters[nearestClass], featureData, x, y);

        for (int j = 1; j < classCenters.size(); ++j)
        {
            distance = d2(classCenters[j], featureData, x, y);
            if (distance < minDistance)
            {
                minDistance = distance;
                nearestClass = j;
            }
        }
        // Assign the point to the nearest class.
        labels(x, y) = nearestClass;
    }
}

/*
    Computing class centers.
    featureData  : Feature vectors (x, y, dim)
    label        : Classes of the points (x, y)
    classCenters : Class centers (ncl, dim)
*/
void RecomputeClassCenters(CImg<> &featureData, CImg<unsigned int> &labels, CImgList<> &classCenters)
{
    CImg<int> pointsPerClass(classCenters.size());
    pointsPerClass.fill(0);

    // Initialize class centers to zero.
    cimg_forX(classCenters, i)
        classCenters[i]
            .fill(0);

    // Sum up feature vectors for each class.
    cimg_forXY(labels, x, y)
    {
        unsigned int assignedClass = labels(x, y);
        cimg_forX(classCenters[assignedClass], dim)
            classCenters[assignedClass](dim) += featureData(x, y, dim);

        ++pointsPerClass(assignedClass);
    }

    // Average to find new centers.
    cimg_forX(classCenters, i)
        classCenters[i] /= pointsPerClass(i);
}

/*
    K-means stopping criterion.
    featureData  : Feature vectors (x, y, dim)
    classCenters : Class centers (ncl, dim)
    label        : Classes of the points (x, y)
*/
float TotalWithinClusterVariance(CImg<> &featureData, CImgList<> &classCenters, CImg<unsigned int> &labels)
{
    float totalVariance = 0;
    cimg_forXY(featureData, x, y)
        totalVariance += d2(classCenters[labels(x, y)], featureData, x, y);
    return totalVariance;
}

/*
    K-means algorithm.
    imgIn      : Input image
    numClasses : Number of classes (ncl)
*/
CImg<unsigned int> PerformKMeans(CImg<> &inputImage, int numClasses)
{
    CImg<> features = ComputeFeatures(inputImage);
    CImg<unsigned int> outputImage(features.width(), features.height());
    float previousVariance, currentVariance;

    // Initialize class centers.
    CImgList<> classCenters(numClasses, features.depth());
    cimg_forX(classCenters, i)
    {
        int x = rand() % features.width();
        int y = rand() % features.height();
        cimg_forX(classCenters[i], dim)
            classCenters[i](dim) = features(x, y, dim);
    }

    // Initial point-to-class assignment.
    AssignToNearestClass(features, classCenters, outputImage);
    currentVariance = TotalWithinClusterVariance(features, classCenters, outputImage);
    float convergenceThreshold = 1e-3f;

    do
    {
        previousVariance = currentVariance;
        AssignToNearestClass(features, classCenters, outputImage);
        RecomputeClassCenters(features, outputImage, classCenters);
        currentVariance = TotalWithinClusterVariance(features, classCenters, outputImage);
    } while (abs(currentVariance - previousVariance) / previousVariance > convergenceThreshold);

    return outputImage;
}

int main()
{
    for (int numClusters = 2; numClusters < 9; numClusters++)
    {
        CImg<> img("../images/lighthouse.png");
        img.norm().blur(0.75f).normalize(0, 255);
        CImg<unsigned int> clusteredImg = PerformKMeans(img, numClusters);
        clusteredImg.normalize(0, 255);
        img.append(clusteredImg).save_png(("./results/kmeans_" + std::to_string(numClusters) + ".png").c_str());
    }
    
    return 0;
}
