#include "Quantization.hpp"
#include <math.h>

const std::vector<std::vector<double>> LUMINANCE_QUANTIZATION_MATRIX = {
    {16, 11, 10, 16, 24, 40, 51, 61},
    {12, 12, 14, 19, 26, 58, 60, 55},
    {14, 13, 16, 24, 40, 57, 69, 56},
    {14, 17, 22, 29, 51, 87, 80, 62},
    {18, 22, 37, 56, 68, 109, 103, 77},
    {24, 35, 55, 64, 81, 103, 113, 92},
    {49, 63, 78, 87, 103, 121, 120, 101},
    {72, 92, 95, 98, 112, 100, 103, 99}
};

const std::vector<std::vector<double>> CHROMA_QUANTIZATION_MATRIX = {
    {17, 18, 24, 47, 99, 99, 99, 99},
    {18, 21, 26, 66, 99, 99, 99, 99},
    {24, 26, 56, 99, 99, 99, 99, 99},
    {47, 66, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99}
};


void quantization(YCbCr_Img_Matrix & transformed_dct_image, uint8_t image_quality) {
    
    for (int row_idx = 0; row_idx < transformed_dct_image.size(); ++row_idx) {
        for (int col_idx = 0; col_idx < transformed_dct_image[0].size(); ++col_idx) {
            transformed_dct_image[row_idx][col_idx].y  = std::round(transformed_dct_image[row_idx][col_idx].y  / LUMINANCE_QUANTIZATION_MATRIX[row_idx%8][col_idx%8]);
            transformed_dct_image[row_idx][col_idx].cb = std::round(transformed_dct_image[row_idx][col_idx].cb / CHROMA_QUANTIZATION_MATRIX[row_idx%8][col_idx%8]);
            transformed_dct_image[row_idx][col_idx].cr = std::round(transformed_dct_image[row_idx][col_idx].cr / CHROMA_QUANTIZATION_MATRIX[row_idx%8][col_idx%8]);
        }
    }
}