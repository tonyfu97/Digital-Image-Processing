# Filtering - Learning Reflection

**Author**: Tony Fu  
**Date**: August 23, 2023  
**Device**: MacBook Pro 16-inch, Late 2021 (M1 Pro)  
**Code**: [GitHub](https://github.com/tonyfu97/Digital-Image-Processing/tree/main/06_feature_extraction)  
**Reference**: Chapter 6 [*Digital Image Processing with C++: Implementing Reference Algorithms with the CImg Library* by Tschumperlé, Tilmant, Barra](https://www.amazon.com/Digital-Image-Processing-Implementing-Algorithms/dp/1032347538)

## 1. Harris-Stephens Corner Detector

I love the explanation of the Corner detection by Professor Shree K. Nayar ([Video link](https://youtu.be/Z_HwkG90Yvw)). Here is the summary:

The Harris-Stephens corner detection algorithm is designed to identify regions where there are substantial variations in gradient in two distinct directions. While it might be simple to detect corners aligned with the x and y axes, real-world images present corners at various angles. This challenge is addressed by using the determinant, which gives a measure of how well-separated the two gradient directions are. A larger determinant typically signifies a strong presence of two different gradient directions. However, an excessively large axis in one direction could mislead the interpretation. To counterbalance this, the algorithm subtracts a term related to the trace, squared and weighted by a parameter, from the determinant. This penalizes the response value \(R\) and aids in distinguishing between genuine corners and edges or flat regions, leading to more accurate corner detection. Here are the steps:

### 1.1 Gradient Calculation

The gradient of the input image is calculated using:
\[ \text{{gradXY}} = \text{{imgIn.get_gradient()}}; \]
This gets the gradients in the x and y directions.

### 1.2 Structure Tensor

```cpp
CImg<>
    Ixx = gradXY[0].get_mul(gradXY[0]).get_convolve(G),
    Iyy = gradXY[1].get_mul(gradXY[1]).get_convolve(G),
    Ixy = gradXY[0].get_mul(gradXY[1]).get_convolve(G);
```

The structure tensor is computed as:
\[
\begin{align*}
I_{xx} &= \text{{gradXY[0].get_mul(gradXY[0]).get_convolve(G)}}, \\
I_{yy} &= \text{{gradXY[1].get_mul(gradXY[1]).get_convolve(G)}}, \\
I_{xy} &= \text{{gradXY[0].get_mul(gradXY[1]).get_convolve(G)}}.
\end{align*}
\]
 
where the Gaussian kernel \(G\) is defined as:
\[ G(x, y) = \frac{1}{2\pi\sigma^2} e^{-\frac{x^2 + y^2}{2\sigma^2}} \]

Together, we can build the structure tensor \(M\) as:

$$
M = \begin{bmatrix} I_{xx} & I_{xy} \\ I_{xy} & I_{yy} \end{bmatrix}
$$

The determinant of \(M\) (\(\det(M)\)) and the trace of \(M\) (\(\text{trace}(M)\)) are computed as follows:

$$\det(M) = I_{xx} \cdot I_{yy} - I_{xy} \cdot I_{xy}$$

$$\text{trace}(M) = I_{xx} + I_{yy}$$

The structure tensor \(M\) plays a key role in feature detection as it represents the distribution of gradients within a specific neighborhood around a point. Rather than directly comparing the gradient of a pixel with those of its neighbors, we use a Gaussian function to calculate an average gradient across an area.

In essence, the structure tensor captures the underlying geometric structure in the vicinity of each pixel. It accomplishes this by portraying gradient orientations as an ellipse in the (\(I_x, I_y\)) plane within a specific window. Here, the determinant is directly proportional to the area of the ellipse, while the trace is equivalent to the sum of the lengths of the ellipse's major and minor axes.

- **Presence of an edge**: When an image contains an edge, the distribution of gradients forms a slender, elongated ellipse. This happens because the intensity changes consistently in one direction (along the edge) and shows little to no change in the direction perpendicular to it. The major axis of this ellipse aligns with the direction of the edge.

- **Presence of a corner**: If a corner is present, the gradients are distributed more evenly, resulting in an elliptical shape that resembles a circle. This is because a corner features significant intensity changes in multiple directions.

- **Flat region**: In a flat region of the image, where there is minimal change in intensity in any direction, the ellipse is small, signaling the absence of distinctive features.

### 1.3 R Function Calculation

```cpp
CImg<>
    det = Ixx.get_mul(Iyy) - Ixy.get_sqr(),
    trace = Ixx + Iyy,
    R = det - k * trace.get_sqr();
```

Often, in the theoretical explanation of the Harris-Stephens corner detection algorithm, we will see the eigenvalues \(\lambda_1\) and \(\lambda_2\) are often introduced to provide an intuitive understanding of the underlying geometric properties of the image. However, in the actual implementation, you can compute the response function \(R\) directly from the components of the second-moment matrix \(I_{xx}\), \(I_{yy}\), and \(I_{xy}\), without having to explicitly calculate the eigenvalues. It is given as:
\[ R = \det - \, k \cdot \text{{trace}}^2 = (I_{xx} \cdot I_{yy} - I_{xy}^2) - k \cdot (I_{xx} + I_{yy})^2\]

| Condition          | Region Type | Explanation                                                                                                                                                 |
|--------------------|-------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------|
| R is close to 0    | Flat Region | No significant change in intensity in any direction, both eigenvalues of the structure tensor are small.                                                     |
| R is small      | Edge        | Significant change in intensity in one direction but not the other, one large and one small eigenvalue of the structure tensor.                               |
| R is positive      | Corner      | Significant changes in intensity in both directions, both eigenvalues of the structure tensor are large, indicating two dominant and different edge directions. |

### 1.4 Local Maxima Detection

```cpp
CImgList<> imgGradR = R.get_gradient();
CImg_3x3(I, float);
CImg<> harrisValues(imgIn.width() * imgIn.height(), 1, 1, 1, 0);
CImg<int>
    harrisXY(imgIn.width() * imgIn.height(), 2, 1, 1, 0),
    perm(imgIn.width() * imgIn.height(), 1, 1, 1, 0);
int nbHarris = 0;
cimg_for3x3(R, x, y, 0, 0, I, float)
{
    if (imgGradR[0](x, y) < eps && imgGradR[1](x, y) < eps)
    {
        float
            befx = Ipc - Icc,
            befy = Icp - Icc,
            afty = Icn - Icc,
            aftx = Inc - Icc;
        if (befx < 0 && befy < 0 && aftx < 0 && afty < 0)
        {
            harrisValues(nbHarris) = R(x, y);
            harrisXY(nbHarris, 0) = x;
            harrisXY(nbHarris++, 1) = y;
        }
    }
}
```

Local maxima of the \(R\) function are detected. This part of the code finds points that are potential corners.

### 1.5 Sorting the Corners

```cpp
harrisValues.sort(perm, false);
```

The values are sorted, and the top \( n \) corners are drawn on the image. In other implementations, this step is usually replaced by non-maximum suppression.

![harris](./results/06/lighthouse_harris.png)

## 2. Shi-Tomasi Algorithm

Shi-Tomasi algorithm uses similar techniques to compute eigenvalues that represent the local structure of the image, but it applies a different criteria to determine if a region is a corner:

$$
R = \min(\lambda_1, \lambda_2)
$$

The algorithm is implemented as follows:

```cpp
CImg<>
    det = Ixx.get_mul(Iyy) - Ixy.get_sqr(),
    trace = Ixx + Iyy,
    diff = (trace.get_sqr() - 4 * det).sqrt(),
    lambda1 = (trace + diff) / 2,
    lambda2 = (trace - diff) / 2,
    R = lambda1.min(lambda2);
```

![shi-tomasi](./results/06/lighthouse_shi_tomasi.png)

Shi-Tomasi's reliance on the minimum eigenvalue often leads to better detection of true corners. Not sure about this one.

## 3. SIFT
