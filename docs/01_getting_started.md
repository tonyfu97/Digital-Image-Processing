# Getting Started with the CImg Library - Learning Reflection

**Author**: Tony Fu  
**Date**: August 18, 2023  
**Device**: MacBook Pro 16-inch, Late 2021 (M1 Pro)  
**Reference**: Chapter 2.1 - 2.2 [*Digital Image Processing with C++: Implementing Reference Algorithms with the CImg Library* by Tschumperlé, Tilmant, Barra](https://www.amazon.com/Digital-Image-Processing-Implementing-Algorithms/dp/1032347538)
**Code**: [GitHub](https://github.com/tonyfu97/Digital-Image-Processing/tree/main/01_getting_started)

## 1. Installing XQuartz (X11 on macOS)

Yes, I lied. I said you only need to include the `CImg.h` header file to get started. But that's not entirely true. You also need to install the `X11` library, which is used to display images.

The `X11` library is used to display images. It is an open-source effort to develop a version of the X.Org X Window System that runs on macOS. You can download the latest version of XQuartz [here](https://www.xquartz.org). Follow the instructions to install it on your machine.

Because `X11` is often installs in a non-standard location (/opt/X11). This means that the compiler and linker may not automatically look in this location when trying to find the X11 libraries and include files. So, we need to modified the `~/.zshrc` file (or whatever shell you're using) to add the following lines:

```bash
# XQuartz (for CImg)
export LIBRARY_PATH=/opt/X11/lib:$LIBRARY_PATH
export CPATH=/opt/X11/include:$CPATH
```
This effectively includes the `X11` library in the compiler's search path. Don't forget to restart the shell or execute `source ~/.zshrc` to apply the changes to the current shell.

## 2. Installing the PNG Library

If you want to read and save images in .png format, as demonstrated in the upcoming example, you'll need to install the `libpng` library. You can install it using Homebrew with the following command:

```bash
brew install libpng
```

You won't need to modify any environment variables for `libpng`. When you install `libpng` using Homebrew (or another standard package manager), it places the library and include files in standard locations that the compiler and linker already recognize.

## 3. First Program

To display an image using the CImg library, include the `CImg.h` header file and use the `CImgDisplay` class. Here's a simple example `first_code.cpp`:

```cpp
#define cimg_use_png
#include "CImg.h"
using namespace cimg_library;

int main()
{
    CImg<unsigned char> img("../images/lighthouse.png");
    img.display("Lighthouse");

    return 0;
}
```

The `CImg` class is a template class that represents an image. The template parameter specifies the type of the image pixels. In this case, the image is a color image with unsigned 8-bit integer pixels. The `CImg` class has a constructor that takes a filename as input and loads the image from the file. The `display()` method of the `CImg` class displays the image in a window. The first argument is the title of the window.

Here the macro `cimg_use_png` is used to specify that the `libpng` library should be used to read and write images in .png format. This gives us a peak into how we can customize the CImg library to suit our needs.

To compile the program, you need to link the `X11` and `png` library. Here is the command I used:

```bash
g++ -o first_code first_code.cpp -lX11 -lpthread -lpng
```

The `-lpthread` option links the `pthread` library. The `pthread` library is used to create threads, which is needed by the `CImg` library.

To run the program, do the following:

```bash
./first_code
```

## 4. Visual Studio Code Configuration

If you're coding inside Visual Studio Code like I am, you might see a red squiggly line under `#include "CImg.h"`, accompanied by the complaint `Cannot open source file "X11/Xlib.h" (dependency of "CImg.h")`. Here's how to fix this issue:

1. Open the Command Palette by pressing `Cmd+Shift+P` on a Mac or `Ctrl+Shift+P` on Windows/Linux.
   
2. Type "C/C++" in the Command Palette, and then select "Edit Configurations (JSON)" from the dropdown list. This action will open the `c_cpp_properties.json` file directly.

3. Add the following line to the `includePath` array:

```json
"includePath": [
    "${workspaceFolder}/**",
    "/opt/X11/include/**"  // Add this line
],
```

4. Save the file. The red squiggly line should now disappear.
