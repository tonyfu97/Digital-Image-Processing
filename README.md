# Digital Image Processing
This repository is dedicated to studying image processing algorithms using C++ and the CImg Library. 

## Introduction
I used to rely on OpenCV for image processing tasks. It's powerful, but getting it set up on a new computer was always a bit of a headache. Plus, it has so many features that it was hard to keep track of everything.

Then I found CImg. What's cool about it is that it's all in one header file. That means you can just drop it into your project and get going. I'll be sharing what I learn as I go along, so check out my [study notes](https://tonyfu97.github.io/Digital-Image-Processing/) if you're interested.

## References
Here are some key resources that guided this project:
- **Primary Reference**: [*Digital Image Processing with C++: Implementing Reference Algorithms with the CImg Library* by Tschumperlé, Tilmant, Barra](https://www.amazon.com/Digital-Image-Processing-Implementing-Algorithms/dp/1032347538)
- [CImg Library](http://cimg.eu/)
- [*Principles of Digital Image Processing* series by Burger &amp; Burge (2009, 2013)](https://imagingbook.com/books/englisch-edition-3-vol-softcover/)

## Core Concepts

| Chapter | Topic | Key Ideas |
|---------|-------|-----------|
| 1 | [Getting Started](./01_getting_started/) | Installation, hello world, and `CImgDisplay` |
| 2 ![lighthouse_block](./docs/results/02/lighthouse_blocks.png)| [Block Decomposition](./02_block_decomposition/) | `CImg` template class, reading command-line parameters, get vs. non-get methods, block decomposition, loops in CImg, draw methods |
| 3 ![hist_equalized](./docs/results/03/hist_equalized.png)| [Point Processing Transformations](./03_point_processing/) | Mathematical transformations, bitwise transformations, histogram equalization |
| 4 ![coins_sa_filtering11](./docs/results/04/coins_sa_filtering11.png)| [Mathematical Morphology](./04_mathematical_morphology/) | Dilation, erosion, opening, closing, Kramer-Bruckner, Alternating Sequential Filters (ASF), morphological gradients, Beucher gradient, skeletonization |
| 5 ![lighthouse_gradient_norm](./docs/results/05/lighthouse_gradient_norm.png)| [Filtering](./05_filtering/) | Convolution, median filter, first-order derivative, second-order derivative, adaptive filters, adaptive window filters, frequency-domain filtering, diffusion filtering | 
| 6 ![road_hough_thresholded_0.90](./docs/results/06/road_hough_thresholded_0.9.png)| [Feature Extraction](./06_feature_extraction/) | Harris-Stephens corner detection, Shi-Tomasi algorithm, Hough transform, texture spectrum, Tamura texture coefficients, Local Binary Patterns (LBP) |
| 7 ![active_contours_400](./docs/results/07/active_contours_400.png)| [Segmentation](./07_segmentation/) | Active contours, Ostu's algorithm, Bernsen's algorithm, K-means clustering, Simple Linear Iterative Clustering (SLIC) |
| 8 | [Motion Estimation](./08_motion/) | |
| 9 | [Multisppectral Imaging](./09_multispectral/) | |
| 10 | [3D Visualization](./10_3d/) | |
| Appendix 1| [Math Expressions in CImg's Fill Method](./appendix_1/) | How to write math expression for `CImg<>::fill()` |
| Appendix 2| [JPEG Compression](./appendix_2/) | |
