#include <iostream>

#define cimg_use_png
#include <CImg.h>

using namespace cimg_library;

const unsigned int N = 8; // Resolution of a block

/**
    JPEG_DCT: Compute the discrete cosine transform (DCT)
    @param block     : Input image
    @param cosvalues : Pre-calculation of the cosine values for DCT
    @return DCT of the input image
**/
CImg<> JPEG_DCT(CImg<> &block, CImg<> &cosvalues)
{
    CImg<> dct(N, N, 1, 1, 0);
    cimg_forXY(dct, i, j)
    {
        float
            ci = i == 0 ? 1 / sqrt(2.0f) : 1,
            cj = j == 0 ? 1 / sqrt(2.0f) : 1;
        cimg_forXY(block, x, y)
            dct(i, j) += block(x, y) * cosvalues(x, i) * cosvalues(y, j);
        dct(i, j) *= 2.0f / N * ci * cj;
    }
    return dct;
}

/**
     JPEG_IDCT: Compute the inverse discrete cosine transform (iDCT).
    @param dct       : DCT <=> input data
    @param cosvalues : Pre-calculation of the cosine values for DCT
    @return Output image
**/
CImg<> JPEG_IDCT(CImg<> &dct, CImg<> &cosvalues)
{
    CImg<> img(N, N, 1, 1, 0);
    cimg_forXY(img, x, y)
    {
        cimg_forXY(dct, i, j)
        {
            float
                ci = (i == 0) ? 1 / std::sqrt(2) : 1,
                cj = (j == 0) ? 1 / std::sqrt(2) : 1;
            img(x, y) += ci * cj * dct(i, j) * cosvalues(x, i) * cosvalues(y, j);
        }
        img(x, y) *= 2.0f / N;
    }
    return img;
}

/**
     JPEGEncoder: Compute the DCT transform + Quantization
    @param image     : Input image
    @param quality   : Quality factor / Compression ratio ( low = higher quality )
    @param cosvalues : Pre-calculation of the cosine values for DCT
    @return Image of the quantized DCT
**/
CImg<> JPEGEncoder(CImg<> &image, float quality, CImg<> &cosvalues)
{
    // Quantization matrix from the JPEG standard.
    int dataQ[] = {16, 11, 10, 16, 24, 40, 51, 61,
                   12, 12, 14, 19, 26, 58, 60, 55,
                   14, 13, 16, 24, 40, 57, 69, 56,
                   14, 17, 22, 29, 51, 87, 80, 62,
                   18, 22, 37, 56, 68, 109, 103, 77,
                   24, 35, 55, 64, 81, 104, 113, 92,
                   49, 64, 78, 87, 103, 121, 120, 101,
                   72, 92, 95, 98, 112, 100, 103, 99};
    CImg<>
        Q = CImg<>(dataQ, N, N) * quality,
        comp(image.width(), image.height(), 1, 1, 0),

        block(N, N), dct(N, N);
    for (int k = 0; k < image.width() / N; ++k)
        for (int l = 0; l < image.height() / N; ++l)
        {
            block = image.get_crop(k * N, l * N, (k + 1) * N - 1, (l + 1) * N - 1);
            block -= 128;
            dct = JPEG_DCT(block, cosvalues);
            cimg_forXY(dct, i, j)
                comp(k * N + i, l * N + j) = cimg::round(dct(i, j) / Q(i, j));
        }
    return comp;
}

/**
    JPEGDecoder: Compute the reconstructed image from the quantized DCT
    @param img_dct   : Quantized DCT <=> Input data
    @param quality   : Quality factor / Compression ratio ( low = higher quality )
    @param cosvalues : Pre-calculation of the cosine values for DCT
    @return Reconstructed image
**/
CImg<> JPEGDecoder(CImg<> &img_dct, float quality, CImg<> &cosvalues)
{
    // Quantization matrix, from the JPEG standard.
    int dataQ[] = {16, 11, 10, 16, 24, 40, 51, 61,
                   12, 12, 14, 19, 26, 58, 60, 55,
                   14, 13, 16, 24, 40, 57, 69, 56,
                   14, 17, 22, 29, 51, 87, 80, 62,
                   18, 22, 37, 56, 68, 109, 103, 77,
                   24, 35, 55, 64, 81, 104, 113, 92,
                   49, 64, 78, 87, 103, 121, 120, 101,
                   72, 92, 95, 98, 112, 100, 103, 99};
    CImg<>
        Q = CImg<>(dataQ, N, N) * quality,
        decomp(img_dct.width(), img_dct.height(), 1, 1, 0),
        dct(N, N), blk(N, N);
    for (int k = 0; k < img_dct.width() / N; ++k)
        for (int l = 0; l < img_dct.height() / N; ++l)
        {
            dct = img_dct.get_crop(k * N, l * N, (k + 1) * N - 1, (l + 1) * N - 1);
            dct.mul(Q);
            blk = JPEG_IDCT(dct, cosvalues) + 128;
            cimg_forXY(blk, i, j)
                decomp(k * N + i, l * N + j) = blk(i, j);
        }
    return decomp;
}

/**
    distortionRate : Compute the quadratic deviation.
    @param image      : Original image
    @param comp_image : "Compressed" image (DCT + quantization)
    @return Quadratic deviation
**/
float distortionRate(CImg<> &image, CImg<> &comp_image)
{
    float rate = (image - comp_image).sqr().sum();
    return rate /= image.width() * image.height();
}

/**
    genCosValues : Pre-calculation of the cosine values for DCT
    @return Cosine values
**/
CImg<> genCosValues()
{
    CImg<> cosinusvalues(N, N);
    cimg_forXY(cosinusvalues, i, x)
        cosinusvalues(x, i) = std::cos(((2 * x + 1) * i * cimg::PI) / (2 * N));
    return cosinusvalues;
}

int main(int argc, char **argv)
{
    // Passing parameters by command line.
    cimg_usage("Simplified JPEG compression");
    const char
        *input_f = cimg_option("-i", "../images/street.png", "Input image"),
        *output_f = cimg_option("-o", "./results/jpeg_street_50.png", "Output image");
    float quality = cimg_option("-q", 50.0, "Quality factor");
    // Read input image.
    CImg<> imgIn(input_f);
    imgIn.norm().normalize(0, 255).save("./results/input_image.png");
    // Pre-compute cosine values.
    CImg<> cos_values = genCosValues();
    cos_values.get_normalize(0, 255).resize(256, 256).save("./results/cos_values.png");
    // Compute DCT + quantization.
    CImg<> dct_image = JPEGEncoder(imgIn, quality, cos_values);
    // Image reconstruction.
    CImg<> comp_image = JPEGDecoder(dct_image, quality, cos_values);
    // Display quadratic deviation between the images.
    float dist = distortionRate(imgIn, comp_image);
    std::cout << "Distortion Rate : " << dist << std::endl;
    // Display images.
    (imgIn, dct_image, comp_image).display("Input image - "
                                           "Image of the DCT blocks - "
                                           "Decompressed image");
    // Save image.
    if (output_f)
        comp_image.normalize(0, 255).save(output_f);
}