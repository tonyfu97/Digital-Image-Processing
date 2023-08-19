# Block Decomposition - Learning Reflection

**Author**: Tony Fu  
**Date**: August 18, 2023  
**Device**: MacBook Pro 16-inch, Late 2021 (M1 Pro)  
**Reference**: Chapter 2.3 - 2.7 [*Digital Image Processing with C++: Implementing Reference Algorithms with the CImg Library* by Tschumperlé, Tilmant, Barra](https://www.amazon.com/Digital-Image-Processing-Implementing-Algorithms/dp/1032347538)

## 1. `CImg` Template Class

The `CImg` library is a template-based image manipulation library, and its template argument specifies the pixel type. By default, if you don't specify the template argument, it's instantiated with `float`.

So when you declare an image like this:

```cpp
CImg<> img("image.png");
```

It is equivalent to:

```cpp
CImg<float> img("image.png");
```

You can specify a different type if you want, such as `unsigned char`, `int`, etc. But if you simply use `CImg<>`, then it defaults to using `float`.

## 2. Reading Command-Line Parameters

The `cimg_usage()` and `cimg_option()` functions are used to handle command-line arguments. Here's a brief description of each function:

1. **`cimg_usage(const char *const format, ...)`:** This function is typically used to print a description of your program when it's invoked from the command line.

2. **`cimg_option(const char *const opt, type_def variable, const char *const format, ...)`:** This function is a command-line option parser. It's used to handle options passed to your program when it's invoked from the command line. Here's a breakdown of the parameters:
   - `opt`: the name of the command-line option.
   - `variable`:  the default value that will be assigned to the variable if the corresponding command-line option is not provided.
   - `format`: a string that may contain a description of what the option does (this will be printed if a specific help option is invoked, like `--help`).

Here's a simple example showing how you might use these functions:

```cpp
#include "CImg.h"

int main(int argc, char **argv) {
    cimg_usage("My simple program that does XYZ.");
    int my_option1 = cimg_option("-o1", 0, "An optional parameter that affects behavior.");
    int my_option2 = cimg_option("-o2", 99, "Another optional parameter that affects behavior.");
    // Rest of the program
}
```

If the user runs the program with the options, like `./my_program -o1 5`, the `my_option1` variable will be set to `5`, and the `my_option2` variable will be set to the default value of `99`.

If they run the program with the `--help` option, they will see the usage string followed by the options descriptions.

## 3. Get vs. Non-Get Methods in Image Processing with CImg

In image processing using CImg, it's really helpful to know whether a method is going to give you a new object (a get method) or change the one you already have (a non-get method). CImg has both types for most of its methods:

* **Get Methods**: These create a new object with the changes you want, leaving the one you started with the same. Like `CImg<float>::get_blur()`, which makes a new blurred image but doesn't touch the original.
* **Non-get Methods**: These change the object you call them on and usually give you back a reference to that changed object. For example, `CImg<float>::blur()` changes the image and gives you back a reference to it.

Here's an example to show how this works:

```cpp
CImg<> lum = img.get_norm().blur(sigma).normalize(0, 255);
```

In this code, `get_norm` makes a new image (make it gray-scale by taking the L2-norm of the RGB channel), and `blur` and `normalize` change it and give you back references. This way of doing things makes it easy to chain operations together and save memory.

**Resutls:**

* Original Image
![lighthouse original](images/lighthouse.png) 

* Luminance Image
![lighthouse lum](results/02/lighthouse_lum.png)

## 4. Gradient Magnitude Computation

Gradient is computed using the [`get_gradient()`](https://cimg.eu/reference/structcimg__library_1_1CImg.html#a0f7d2161b942a08e4575451de817227d) method. The gradient is computed using the *centered finite differences* by default. 

I have explained more on each of the gradient computation methods [here](gradient_computation.md).

