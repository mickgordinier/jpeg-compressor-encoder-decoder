#include "YCbCr_Transformation.hpp"

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
YCbCr_Img_Matrix convert_RGB_to_padded_YCbCr(const RGB_Img_Matrix & rgb_matrix) {
    YCbCr_Img_Matrix yCbCr_matrix;

    // NEED HEIGHT AND WIDTH TO BE MULTIPLE OF 8 TO PROPERLY PERFORM DCT
    // WILL IGNORE PADDED PIXELS AFTERWARDS
    uint32_t yCbCr_height = rgb_matrix.size();
    uint32_t yCbCr_width = rgb_matrix[0].size();

    if (yCbCr_height % 8 != 0) {
        yCbCr_height += (8 - yCbCr_height%8);
    }

    if (yCbCr_width % 8 != 0) {
        yCbCr_width += (8 - yCbCr_width%8);
    }

    // Resizing yCbCr matrix
    yCbCr_matrix.resize(yCbCr_height);

    for (int i = 0; i < yCbCr_matrix.size(); ++i) {
        yCbCr_matrix[i].resize(yCbCr_width);
    }

    // Converting all rgb values to yCbCr values
    /*
        Source: https://www.w3.org/Graphics/JPEG/jfif.pdf
        "The color space to be used is YCbCr as defined by CCIR 601 (256 levels). The RGB
        components calculated by linear conversion from YCbCr shall not be gamma corrected
        (gamma = 1.0). If only one component is used, that component shall be Y."

        ...

        RGB to YCbCr Conversion
        YCbCr (256 levels) can be computed directly from 8-bit RGB as follows:

        Y = 0.299 R + 0.587 G + 0.114 B
        Cb = - 0.1687 R - 0.3313 G + 0.5 B + 128
        Cr = 0.5 R - 0.4187 G - 0.0813 B + 128
    */

    for (uint32_t row_idx = 0; row_idx < rgb_matrix.size(); ++row_idx) {
        for (uint32_t col_idx = 0; col_idx < rgb_matrix[0].size(); ++col_idx) {
            
            RGB_Val rgb_val = rgb_matrix[row_idx][col_idx];

            // Notice: Information will be lost as it will round down to integer
            YCbCr_Val yCbCr_val;
            
            // luminance (Y) channel
            yCbCr_val[0] = (rgb_val[0] * 0.299) + (rgb_val[1] * 0.587) + (rgb_val[2] * 0.114);
            
            // chroma blue (Cb) channel
            yCbCr_val[1] = (rgb_val[0] * -0.1687) + (rgb_val[1] * -0.331) + (rgb_val[2] * 0.5) + 128;
            
            // chroma red (Cr) channel
            yCbCr_val[2] = (rgb_val[0] * 0.5) + (rgb_val[1] * -0.4187) + (rgb_val[2] * -0.0813) + 128;

            yCbCr_matrix[row_idx][col_idx] = yCbCr_val;
        }
    }

    pad_YCbCr(yCbCr_matrix, rgb_matrix.size(), rgb_matrix[0].size());

    return yCbCr_matrix;
}

// Padding with repeated pixels technique to make image size multiple of 8
void pad_YCbCr(YCbCr_Img_Matrix & yCbCr, uint32_t original_height, uint32_t original_width) {

    /*
    Source: https://www.thewebmaster.com/jpeg-definitive-guide/

    Block splitting is the process of splitting each channel in a source image into pixel blocks called Minimum Coded Units (MCU).
    Each channel may have a different-sized MCU, depending on whether that channel was subsampled.

    For example, here is the MCU size for the following subsampling ratios:
    4:4:4 - 8 x 8 pixels
    4:2:2 - 16 * 8 pixels
    4:2:0 - 16 * 16 pixels

    ...

    For JPEG compression algorithms to work, images must only contain complete MCU blocks.
    In other words, the image dimensions must be multiples of an MCU, for example, 8 pixels for the 4:4:4 subsampling ratio. 
    For example, an image with a size of 501 x 375 pixels must first be changed to an image with a size of 504 x 376 pixels.

    If the image has partial MCU blocks, then the software encoding the JPEG image must add its own dummy data to those blocks.

    The data (or padding) added to the image depends on the software, but the following techniques may be used:
    1. Use a fixed color, such as black.
    !!!!!!!!!  2. Repeat the pixels at the edge of the image (the most common technique).   !!!!!!!!!
    3. More sophisticated techniques.

    Once the dummy data is added, the blocks go through the rest of the JPEG encoding process.

    Finally, the additional pixels are removed.
    */

    // Pad same pixels through the padded columns
    for (uint32_t col_idx = original_width; col_idx < yCbCr[0].size(); ++col_idx) {
        for (uint32_t row_idx = 0; row_idx < original_height; ++row_idx) {
            YCbCr_Val value_to_copy = yCbCr[row_idx][original_width-1];

            yCbCr[row_idx][col_idx][0] = value_to_copy[0];
            yCbCr[row_idx][col_idx][1] = value_to_copy[1];
            yCbCr[row_idx][col_idx][2] = value_to_copy[2];
        }
    }

    // Pad same pixels through the padded rows
    for (uint32_t row_idx = original_height; row_idx < yCbCr.size(); ++row_idx) {
        for (uint32_t col_idx = 0; col_idx < original_width; ++col_idx) {
            YCbCr_Val value_to_copy = yCbCr[original_height-1][col_idx];

            yCbCr[row_idx][col_idx][0] = value_to_copy[0];
            yCbCr[row_idx][col_idx][1] = value_to_copy[1];
            yCbCr[row_idx][col_idx][2] = value_to_copy[2];
        }
    }

    // for the corner pixels
    YCbCr_Val value_to_copy = yCbCr[original_height-1][original_width-1];

    for (uint32_t row_idx = original_height; row_idx < yCbCr.size(); ++row_idx) {
        for (uint32_t col_idx = original_width; col_idx < yCbCr[0].size(); ++col_idx) {
            yCbCr[row_idx][col_idx][0] = value_to_copy[0];
            yCbCr[row_idx][col_idx][1] = value_to_copy[1];
            yCbCr[row_idx][col_idx][2] = value_to_copy[2];
        }
    }
}