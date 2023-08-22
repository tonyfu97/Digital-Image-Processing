# Filtering - Learning Reflection

**Author**: Tony Fu  
**Date**: August 21, 2023  
**Device**: MacBook Pro 16-inch, Late 2021 (M1 Pro)  
**Reference**: Chapter 5 [*Digital Image Processing with C++: Implementing Reference Algorithms with the CImg Library* by Tschumperlé, Tilmant, Barra](https://www.amazon.com/Digital-Image-Processing-Implementing-Algorithms/dp/1032347538)

## 1. Convolution

```cpp
CImg<> sobel(3, 3, 1, 1, 0);
sobel(0, 0) = -1; sobel(0, 1) = -2; sobel(0, 2) = -1;
sobel(1, 0) = 0;  sobel(1, 1) = 0;  sobel(1, 2) = 0;
sobel(2, 0) = 1;  sobel(2, 1) = 2;  sobel(2, 2) = 1;
imgIn.convolve(sobel);
```

The above code snippet shows how to perform a convolution on an image with a 3x3 Sobel filter. The result is shown below:

- **Origin**:

![original](../images/lighthouse.png)

- **Convolution**:

![conv](./results/05/lighthouse_conv.png)


### Boundary Conditions

Boundary conditions specify how to handle the edges and can be specified using the `const usigned int boundary_conditions` parameter of the `convolve()` method. The four boundary conditions provided by the CImg library have specific meanings:

1. **Dirichlet (0):** The pixels outside the image boundaries are considered to be zero. This creates a sort of "hard" edge around the image and can lead to noticeable artifacts along the borders.

2. **Neumann (1) (default):** The value of the border pixels is extended outside the image boundaries. Essentially, this reflects the gradient at the border, assuming that the intensity of the image doesn't change beyond the edge. This is the default boundary condition in CImg and tends to provide visually acceptable results.

3. **Periodic (2):** The image is treated as if it were tiling the plane in a repeated pattern. This means that the pixels on the right edge of the image are used as the boundary condition for the left edge, and the pixels on the bottom are used for the top. This can create seamless transitions but can also lead to strange effects if the image does not naturally tile.

4. **Mirror (3):** The pixels outside the image boundaries are determined by mirroring the pixels inside the boundaries. Imagine folding the image over at its edges, so the pixels just inside the border are duplicated just outside the border. This can create a more visually smooth transition at the edges but may not be appropriate for all types of images.


## 2. Median Fitler

```cpp
img.blur_median(3);
```

The above code snippet shows how to perform a median filter on an image with a 3x3 window. The result is shown below:

- **Origin**:

![coins_threshold](../results/05/coins_threshold.png)

- **Median Filter**:

![median](./results/05/coins_median.png)


### Order-Statistic (OS) Filter

An [OS filter (Bovik, Huang, and Munson, 1983)](https://ieeexplore.ieee.org/document/1164247) is a non-linear filter that computes a linear combination of these sorted values:

\[ F = w_1 \cdot s_1 + w_2 \cdot s_2 + \ldots + w_n \cdot s_n \]

where \( w_i \) are the weights that define how much each ordered value contributes to the final result, and \( s_i \) are the sorted values of the neighborhood pixels.


## 3. 