# Multispectral Image Processing - Learning Reflection

**Author**: Tony Fu  
**Date**: August 30, 2023  
**Device**: MacBook Pro 16-inch, Late 2021 (M1 Pro)  
**Code**: [GitHub](https://github.com/tonyfu97/Digital-Image-Processing/tree/main/09_multispectral)  
**Reference**: Chapter 9 [*Digital Image Processing with C++: Implementing Reference Algorithms with the CImg Library* by Tschumperlé, Tilmant, Barra](https://www.amazon.com/Digital-Image-Processing-Implementing-Algorithms/dp/1032347538)

## Principal Component Analysis (PCA) for Reducing Color Channels

Principal Component Analysis (PCA) is a widely-used technique for dimensionality reduction. Here, the book introduces PCA in the context of reducing the number of color channels in images, but it is also frequently used in image processing tasks such as image compression and appearance-based object recognition (not covered). For a more in-depth understanding, please refer to these lectures: [Lecture 1](https://youtu.be/M6fBAzcw1Ps?si=zJfF_4zNEiaa8jnf), [Lecture 2](https://youtu.be/DcngAJXMoRo?si=3P7mRL9JUfmrVyh4).

### Algorithm Walkthrough

#### 1. Data Standardization

The first step is to standardize the data. This is accomplished by subtracting the mean and dividing by the standard deviation:

\[
\mathbf{Z} = \frac{\mathbf{X} - \bar{\mathbf{X}}}{\sigma}
\]

Here, \( \mathbf{X} \) is a column of the data matrix, \( \bar{\mathbf{X}} \) is the mean of \( \mathbf{X} \), and \( \sigma \) is the standard deviation of \( \mathbf{X} \).

#### 2. Calculate the Correlation Matrix

Next, calculate the correlation matrix:

\[
\mathbf{R} = \mathbf{Z}^T \times \mathbf{Z}
\]

Note that usually, the correlation matrix is further divided by the number of samples, but this is not done here.

#### 3. Compute the Eigenvalues and Eigenvectors

Using `CImg<T>::get_symmetric_eigen()`, we can obtain the eigenvalues and eigenvectors of the correlation matrix, sorted in descending order. Mathematically, this involves solving the following equation:

\[
\mathbf{R} \times \mathbf{v} = \lambda \times \mathbf{v}
\]

Here, \( \mathbf{v} \) is the eigenvector and \( \lambda \) is the eigenvalue.

#### 4. Select Principal Components

This step involves selecting the top \( k \) columns from \(\mathbf{v}\). In the code, we use the principal component(s) that account for 90% of the variance in the data.

#### 5. Transform Original Dataset

Finally, transform the original dataset by multiplying it with the selected principal components:

\[
\mathbf{X}_{\text{pca}} = \mathbf{X} \times \mathbf{v}
\]

### Example

Here we perform PCA on the following color image:

![cat](./images/cat.png)

The first principal component, which accounts for over 90% of the variation, is shown below:

![pca_0_cat](./results/09/pca_0_cat.png)

Interestingly, the second principal component highlights the cat toy in the background:

![pca_1_cat](./results/09/pca_1_cat.png)

And here is the third principal component:

![pca_2_cat](./results/09/pca_2_cat.png)


## 2. Color Spaces

| Color Space | Description | Channels | Applications |
|-------------|-------------|----------|--------------|
| RGB         | Red, Green, Blue | 3 | General-purpose, display, image & video capture |
| Grayscale   | Luminance | 1 | Image analysis, older video transmission |
| HSV         | Hue, Saturation, Value | 3 | Image analysis, computer vision, graphics |
| HSL         | Hue, Saturation, Lightness | 3 | Image analysis, computer vision, graphics |
| YCbCr       | Luma, Blue-difference, Red-difference | 3 | Video compression, broadcast |
| Lab         | Lightness, a (Green to Red), b (Blue to Yellow) | 3 | Color conversion, image analysis |
| YUV         | Luminance, Chrominance U, Chrominance V | 3 | Video compression, broadcast |
| CMYK        | Cyan, Magenta, Yellow, Black | 4 | Printing |
| XYZ         | Standardized RGB | 3 | Color science, conversions |
| sRGB        | Standard RGB | 3 | Web, general-purpose graphics |
| L*a*b*      | Lightness, a*, b* | 3 | Color analysis, computer vision |
