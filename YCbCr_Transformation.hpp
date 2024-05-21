#ifndef YCBCR_TRANSFORMATION_HPP
#define YCBCR_TRANSFORMATION_HPP

#include "CommonTypes.hpp"
#include <vector>
#include <array>
#include <cstdint>

// Converting RGB image to YCbCr Image. Also padding image to make dimesions multiple of 8.
/*
    Source: https://www.thewebmaster.com/jpeg-definitive-guide/

    The human eye is much more sensitive to fine variations in brightness (luminance) than to changes in color (chroma).
    JPEG can take advantage of this by converting to the YCbCr color model, which splits the luminance from the chroma. 
    We can then apply stronger compression to the chroma channels with less effect on image quality.

    The YCbCr color model converts from the RGB color model and consists of 3 separate categories:

    Luminance (Y) - The luminance data from each red, green, and blue RGB channel is extracted and separated from the chroma data and combined to make the Luminance (Y) channel. 
                    The Y channel, by itself, contains enough data to create a complete black-and-white image.
    Chroma Blue (Cb) - The Chroma Blue Channel is the RGB blue channel minus the luminance.
    Chroma Red (Cr) - The Chroma Red Channel is the RGB blue channel minus the luminance.
    After subtracting the Y, Cb, and Cb channels from the image, the remainder provides the chroma green information. This can be mathematically extracted instead of encoded, saving bandwidth.
*/
YCbCr_Img_Matrix convert_RGB_to_padded_YCbCr(const RGB_Img_Matrix & rgb_matrix);

// Padding with repeated pixels technique to make image size multiple of 8
void pad_YCbCr(YCbCr_Img_Matrix & yCbCr, uint32_t original_height, uint32_t original_width);


#endif