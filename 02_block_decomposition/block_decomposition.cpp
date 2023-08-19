/*
 Deompose image into rectangular blocks of uniform color.
*/

#include <string>

#define cimg_use_png
#include "CImg.h"

using namespace cimg_library;

int main(int argc, char **argv)
{
    // Read image filename from command line or use default
    cimg_usage("Decomposition into blocks of a color image");
    std::string filename = cimg_option("-i", "../images/lighthouse.png", "Color image");
    std::string file_basename = filename.substr(9, filename.size() - 13);
    float threshold = cimg_option("-t", 25.0f, "Gradient threshold");
    float sigma = cimg_option("-s", 0.7f, "Gradient smoothness");

    // Load the image and shrink it to fit the screen
    CImg<unsigned char> img(filename.c_str());
    while (img.width() >= CImgDisplay::screen_width() ||
           img.height() >= CImgDisplay::screen_height())
    {
        img.resize(-50, -50, 1, -100, 2);
    }

    // Get the luminance of the image and smooth it
    CImg<> lum = img.get_norm().blur(sigma).normalize(0, 255);
    lum.save(("results/" + file_basename + "_lum.png").c_str());

    // Get the gradient norm of the luminance
    CImgList<> grad = lum.get_gradient("xy");
    CImg<> normGrad = (grad[0].get_sqr() += grad[1].get_sqr()).sqrt();
    normGrad.save(("results/" + file_basename + "_normGrad.png").c_str());

    // Initialisation of the list of blocks.
    CImgList<int> blocks;
    CImg<int>::vector(0, 0, img.width() - 1, img.height() - 1).move_to(blocks);

    // Calculation of the decomposition of the image into blocks.
    for (unsigned int l = 0; l < blocks.size();)
    {
        CImg<int> &block = blocks[l];
        int
            x0 = block[0],
            y0 = block[1],
            x1 = block[2], y1 = block[3];
        if (normGrad.get_crop(x0, y0, x1, y1).max() > threshold &&
            std::min(x1 - x0, y1 - y0) > 8)
        {
            int xc = (x0 + x1) / 2, yc = (y0 + y1) / 2;
            CImg<int>::vector(x0, y0, xc - 1, yc - 1).move_to(blocks);
            CImg<int>::vector(xc, y0, x1, yc - 1).move_to(blocks);
            CImg<int>::vector(x0, yc, xc - 1, y1).move_to(blocks);
            CImg<int>::vector(xc, yc, x1, y1).move_to(blocks);
            blocks.remove(l);
        }
        else
            ++l;
    }

    // Rendering of the decomposition.
    CImg<unsigned char> res(img.width(), img.height(), 1, 3, 0);
    CImg<int> coords(img.width(), img.height(), 1, 4, 0);
    cimglist_for(blocks, l)
    {
        CImg<int> &block = blocks[l];
        int
            x0 = block[0],
            y0 = block[1],
            x1 = block[2], y1 = block[3];
        CImg<unsigned char> color = img.get_crop(x0, y0, x1, y1).resize(1, 1, 1, 3, 2);
        res.draw_rectangle(x0, y0, x1, y1, color.data(), 1);
        coords.draw_rectangle(x0, y0, x1, y1, block.data());
    }

    // Adding black borders.
    res.mul(1 - (res.get_shift(1, 1, 0, 0, 0) - res).norm().cut(0, 1));
    res.save(("results/" + file_basename + "_blocks.png").c_str());

    // Start the interactive viewer.
    CImgDisplay disp(res, "CImg Tutorial: Block Decomposition", 0);
    unsigned char white[] = {255, 255, 255}, black[] = {0, 0, 0};
    while (!disp.is_closed() && !disp.is_keyESC())
    {
        int
            x = disp.mouse_x(),
            y = disp.mouse_y();
        if (x >= 0 && y >= 0)
        {
            int
                x0 = coords(x, y, 0),
                y0 = coords(x, y, 1),
                x1 = coords(x, y, 2), y1 = coords(x, y, 3),
                xc = (x0 + x1) / 2, yc = (y0 + y1) / 2;
            CImg<unsigned char>
                pImg = img.get_crop(x0, y0, x1, y1).resize(128, 128, 1, 3, 1),
                pGrad = normGrad.get_crop(x0, y0, x1, y1).resize(128, 128, 1, 3, 1).normalize(0, 255).map(CImg<unsigned char>::hot_LUT256());
            (+res).draw_text(10, 3, "X, Y = %d, %d", white, 0, 1, 24, x, y).draw_rectangle(x0, y0, x1, y1, black, 0.25f).draw_line(74, 109, xc, yc, white, 0.75, 0xCCCCCCCC).draw_line(74, 264, xc, yc, white, 0.75, 0xCCCCCCCC).draw_rectangle(7, 32, 140, 165, white).draw_rectangle(7, 197, 140, 330, white).draw_image(10, 35, pImg).draw_image(10, 200, pGrad).display(disp);
        }
        disp.wait(); // Wait for an user event
        if (disp.is_resized())
            disp.resize(disp);
    }
    return 0;
}