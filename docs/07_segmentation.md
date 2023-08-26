# Segmentation - Learning Reflection

**Author**: Tony Fu  
**Date**: August 25, 2023  
**Device**: MacBook Pro 16-inch, Late 2021 (M1 Pro)  
**Code**: [GitHub](https://github.com/tonyfu97/Digital-Image-Processing/07_segmentation)  
**Reference**: Chapter 7 [*Digital Image Processing with C++: Implementing Reference Algorithms with the CImg Library* by Tschumperlé, Tilmant, Barra](https://www.amazon.com/Digital-Image-Processing-Implementing-Algorithms/dp/1032347538)

## 1. Active Contours

This chapter is quite math-heavy, so for a more intuitive understanding of the concepts, I recommend checking out Professor Shree Nayar's [lecture](https://youtu.be/FROJUMk9P3Y?si=Bd_46R-5ZORLOjAO) on active contours.

Active contours provide a method for analyzing images and delineating shapes within them. Think of it as placing an elastic band around an object and then letting the band adjust itself to fit the object. In more technical terms, these contours work by minimizing a total energy (i.e., potential energy derived from the image itself and kinetic energy that allows the contour to move and adjust). Initially, you place a starting "loop" or initial contour around the area of interest. The system then works to minimize this total energy.


### 1.1 Initialize Level Set \( \psi \)

Unlike explicit representations like polygonal approximations or parametric curves, the implicit representation—referred to as **level set** (\(\psi\)) representation by the book—offers more flexibility. In this context, a level set is a collection of pixels that are all at the same signed distance from the contour. In particular, the contour is the zero level set, and the pixels inside the contour have negative values, while those outside have positive values. The contour is then the zero crossing of the level set. At the start of the algorithm, we need to initialize the level set. In this case, we can draw a circle centered at \(x_0, y_0\) with radius \(r\). This can be bigger than the object if we are contracting the contour or smaller if we are expanding it. The level set is then initialized as:

$$
\psi_0(x, y) = \sqrt{(x - x_0)^2 + (y - y_0)^2} - r
$$

### 1.2 Define Forces

The level set \( \psi \) is then evolved under the influence of two forces: the propagation force \(F_{prop}\) and the advective force \(F_{adv}\).

#### Stopping Function (Geodesic Model)

A stopping function \(f(x,y)\) is defined to control how fast the contour expands or contracts based on the image gradient.

$$
f(x, y) = exp\_cont \cdot \left( \frac{1}{1 + \| \nabla I(x, y) \|} + balloon \right)
$$

where 

$$
exp\_cont = \begin{cases}
1 & \text{if expanding contour} \\
-1 & \text{if contracting contour}
\end{cases}
$$

and \(balloon\) is a parameter that controls the amount of expansion or contraction.

#### Gradients of Level Set

Here we define two functions \(\nabla ^+\) and \(\nabla ^-\) that takes the level set \(\psi\) as input and returns the gradient of \(\psi\) in the positive and negative directions, respectively. They are defined as:
$$
\nabla ^+ (\psi) = \sqrt{\max(D^{-x}(\psi), 0)^2 + \min(D^{+x}(\psi), 0)^2 + \max(D^{-y}(\psi), 0)^2 + \min(D^{+y}(\psi), 0)^2}\\
\nabla ^- (\psi) = \sqrt{\max(D^{+x}(\psi), 0)^2 + \min(D^{-x}(\psi), 0)^2 + \max(D^{+y}(\psi), 0)^2 + \min(D^{-y}(\psi), 0)^2}
$$

where \(D^{-x}(\psi)\) is the backward difference of \(\psi\) in the x direction, and \(D^{+x}(\psi)\) is the forward difference of \(\psi\) in the x direction. The same applies to the y direction.

#### Propagation Force
The propagation force is then defined as:

$$
F_{prop}(x, y) = - \nabla ^+ (\psi (x, y)) \cdot \max(f(x, y), 0) - \nabla ^- (\psi (x, y)) \cdot \min(f(x, y), 0)
$$

#### Advective Force
The advective force is defined as:

$$
F_{adv}(x, y) = \\
 - \max(\nabla_x (f(x, y)), 0) \cdot \nabla^{-x} (\psi (x, y))\\
 - \min(\nabla_x (f(x, y)), 0) \cdot \nabla ^{+x} (\psi (x, y)) \\
 - \max(\nabla_y (f(x, y)), 0) \cdot \nabla^{-y} (\psi (x, y)) \\
 - \min(\nabla_y (f(x, y)), 0) \cdot \nabla ^{+y} (\psi (x, y))
$$

### 1.3 Evolve Level Set

The level set is then evolved by the following equation:

$$
\psi_{t+1}(x, y) = \psi_t(x, y) + \Delta t \cdot \left( \alpha F_{prop}(x, y) + \beta F_{adv}(x, y) \right)
$$

where \(\alpha\) and \(\beta\) are parameters that control the relative influence of the two forces.

### 1.4 Normalize Level Set

After evolving the level set, we need to normalize it so that the zero level set remains the contour. This is done using the Eikonal equation:

```cpp
if (!(iter % 20))
    LevelSet.distance_eikonal(10, 3);
```

As the contour evolves, numerical irregularities may cause the function to deviate from being a proper signed distance function. Solving the Eikonal equation periodically helps to re-initialize or "normalize" the level set function.

### Example

Here I start with the binarized image of coins:

![active_contours_input](./results/07/active_contours_input.png)

And iteratively apply active contours. I am using expansion here, so the contour is initialized as a smaller circle inside the coin and expands to fit the coin.

![active_contours_0](./results/07/active_contours_0.png)

This is the contour after 40 iterations:

![active_contours_40](./results/07/active_contours_40.png)

After 200 iterations:

![active_contours_200](./results/07/active_contours_200.png)

After 400 iterations:

![active_contours_400](./results/07/active_contours_400.png)

## 2. 