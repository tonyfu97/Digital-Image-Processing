# Point Processing Transformations - Learning Reflection

**Author**: Tony Fu  
**Date**: August 19, 2023  
**Device**: MacBook Pro 16-inch, Late 2021 (M1 Pro)  
**Reference**: Chapter 3 [*Digital Image Processing with C++: Implementing Reference Algorithms with the CImg Library* by Tschumperlé, Tilmant, Barra](https://www.amazon.com/Digital-Image-Processing-Implementing-Algorithms/dp/1032347538)


## 1. Mathematical Transformations

### 1.0. Orignal Image

![original](./results/03/lighthouse_original.png)

### 1.1. Exponential Transformations

```cpp
CImg<> expImg = lum.get_exp()/50;
```
![exponential](./results/03/lighthouse_exp.png)

### 1.2. Square Root Transformations

```cpp
CImg<> sqrtImg = lum.get_sqrt()*10;
```
![sqrt](./results/03/lighthouse_sqrt.png)

### 1.3. Logarithmic Transformations

```cpp
CImg<> logImg = (2 + lum.get_abs()).log();
```
![log](./results/03/lighthouse_log.png)

### 1.4 Cube Transformations

```cpp
CImg<> cubeImg = lum.get_pow(3);
```
![cube](./results/03/lighthouse_cube.png)

## 2. Bitwise Transformations

### 2.0. Orignal Images

![img1](./results/03/img1.png)
![img2](./results/03/img2.png)

### 2.1. Bitwise AND

```cpp
CImg<unsigned char> img_and = img1 & img2;
```
![and](./results/03/and_image.png)

If you have two 8-bit pixels, one with the value 0101 (5 in decimal) and the other with 1010 (10 in decimal), then the bitwise AND operation between these two pixels would give you 0000 (0 in decimal).

### 2.2. Bitwise OR

```cpp
CImg<unsigned char> img_or = img1 | img2;
```
![or](./results/03/or_image.png)

### 2.3. Bitwise XOR

```cpp
CImg<unsigned char> img_xor = img1 ^ img2;
```
![xor](./results/03/xor_image.png)

As we seen in the above example, bitwise operations might seem somewhat abstract when applied to images (5 & 10 = 0. What?), but they are actually quite useful in in specific contexts:

* **Masking**: If you have a binary mask where certain pixels are set to 1 and others are set to 0, you can use the bitwise AND operation with an image to "mask" or isolate those areas. 
* **Steganography or Watermarking**: Bitwise operations might be used to embed or extract information within an image. By operating at the bit level, you can hide data within the least significant bits of an image in a way that's almost visually imperceptible. Refer to Code 3.3 in the book for an example.
* **Thresholding**: If you have two binary images representing different thresholded features, the bitwise AND can be used to find the overlapping areas between those features.

## 3. Histogram Equalization

