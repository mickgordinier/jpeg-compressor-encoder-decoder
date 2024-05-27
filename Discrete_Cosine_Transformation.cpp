#include "Discrete_Cosine_Transformation.hpp"

#include <vector>
#include <cmath>
#include <array>
#include <cstdint>
#include <iostream>

/*
Source: https://www.math.cuhk.edu.hk/~lmlui/dct.pdf
Discrete Cosine Transform (DCT) is commonly used in signal processing and image processing as a method to transform an image from the spatial domain to the frequency domain. 
The idea behind DCT is that any function can be represented by the summation of cosine functions. 
DCT is a reversible process and inherently lossless.
*/

// creates the 8x8 DCT Transformation Matrix
// Source: https://dev.to/marycheung021213/understanding-dct-and-quantization-in-jpeg-compression-1col#:~:text=Discrete%20Cosine%20Transform%20(DCT)%20is,the%20summation%20of%20cosine%20functions.
std::vector<std::vector<double>> create_discrete_cosine_transform_matrix() {
    
    std::vector<std::vector<double>> dct_matrix;

    // Resizing matrix to be 8x8
    dct_matrix.resize(8);

    for (int i = 0; i < 8; ++i) {
        dct_matrix[i].resize(8);
    }

    // entire first row is sqrt(1/N)
    // using double for more precise percision
    double dct_val = std::sqrt(1.0/8.0);
    for (uint8_t col_idx = 0; col_idx < 8; ++col_idx) {
        dct_matrix[0][col_idx] = dct_val;
    }

    // all values multiplied by sqrt(2/8) == sqrt(1/4) == 1/2
    for (uint8_t row_idx = 1; row_idx < 8; ++row_idx) {
        for (uint8_t col_idx = 0; col_idx < 8; ++col_idx) {
            dct_matrix[row_idx][col_idx] = 0.5 * cos(((2*col_idx + 1) * row_idx * M_PI) / 16);
        }
    }

    return dct_matrix;
}

// Performing an in-place DCT transformation on an 8x8 pixel block within the YCbCr Image
// DCT Transformation = (DCT Matrix) * (Original Image) * (DCT Matrix Transformed)
void transform_DCT_8_by_8_block(YCbCr_Img_Matrix & image_to_transform, 
                                const std::vector<std::vector<double>> & dct_matrix, 
                                int img_row_idx, int img_col_idx) 
{
    
    // 1. Make transformed_image = dct_matrix * Original Image (F)
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {

            YCbCr_Val mat_mul_YCbCr;

            for (int i = 0; i < 8; ++i) {
                mat_mul_YCbCr.y  += (dct_matrix[img_row_idx+row][img_col_idx+i] * image_to_transform[img_row_idx+i][img_col_idx+col].y);
                mat_mul_YCbCr.cb += (dct_matrix[img_row_idx+row][img_col_idx+i] * image_to_transform[img_row_idx+i][img_col_idx+col].cb);
                mat_mul_YCbCr.cr += (dct_matrix[img_row_idx+row][img_col_idx+i] * image_to_transform[img_row_idx+i][img_col_idx+col].cr);
            }

            image_to_transform[img_row_idx+row][img_col_idx+col] = mat_mul_YCbCr;
        }
    }

    // 2. Make transformed_image = transformed_image * DCT_Transposed
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            
            YCbCr_Val mat_mul_YCbCr;

            for (int i = 0; i < 8; ++i) {
                // Transposing image DCT_Matrix_Transpose[i][j] = DCT_Matrix[j][i]
                mat_mul_YCbCr.y  += (image_to_transform[img_row_idx+row][img_col_idx+col+i].y * dct_matrix[img_col_idx+col][img_row_idx+row+i]);
                mat_mul_YCbCr.cb += (image_to_transform[img_row_idx+row][img_col_idx+col+i].cb * dct_matrix[img_col_idx+col][img_row_idx+row+i]);
                mat_mul_YCbCr.cr += (image_to_transform[img_row_idx+row][img_col_idx+col+i].cr * dct_matrix[img_col_idx+col][img_row_idx+row+i]);
            }

            image_to_transform[img_row_idx+row][img_col_idx+col] = mat_mul_YCbCr;
        }
    }

}


// Performs DCT Operation
// DCT Transformation = (DCT Matrix) * (Original Image) * (DCT Matrix Transformed)
// Before computing the DCT of each 8x8 block, each value in the matrix must be recentered around zero. I.e., the midpoint must be 0.
void perform_DCT_operation(YCbCr_Img_Matrix & image_to_transform) {

    std::vector<std::vector<double>> dct_matrix = create_discrete_cosine_transform_matrix();
    
    // Need values of YCbCr Image to go from 0-255 --> -128-127 (Subtract 128 to each element)
    for (int row_idx = 0; row_idx < image_to_transform.size(); ++row_idx) {
        for (int col_idx = 0; col_idx < image_to_transform[0].size(); ++col_idx) {
            image_to_transform[row_idx][col_idx].y  -= 128;
            image_to_transform[row_idx][col_idx].cb -= 128;
            image_to_transform[row_idx][col_idx].cr -= 128;
        }
    }
    

    for (int img_row_idx = 0; img_row_idx < image_to_transform.size(); img_row_idx+=8) {
        for (int img_col_idx = 0; img_col_idx < image_to_transform[0].size(); img_col_idx+=8) {
            transform_DCT_8_by_8_block(image_to_transform, dct_matrix, img_row_idx, img_col_idx);
        }
    }
}