#include <vector>
#include <cmath>

typedef std::array<int16_t, 3> YCbCr_Val;               // Needs to be int16_t as values will be shifted down 128 in DCT
typedef std::vector<std::vector<YCbCr_Val>> YCbCr_Img_Matrix;

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
    dct_matrix.resize(8);

    for (int i = 0; i < 8; ++i) {
        dct_matrix[i].resize(8);
    }

    // entire first row is sqrt(1/N)
    // using double for more precise percision
    double dct_val = std::sqrt(1.0/8.0);
    for (std::uint8_t col_idx = 0; col_idx < 8; ++col_idx) {
        dct_matrix[0][col_idx] = dct_val;
    }

    // all values multiplied by sqrt(2/8) == sqrt(1/4) == 1/2
    for (std::uint8_t row_idx = 1; row_idx < 8; ++row_idx) {
        for (std::uint8_t col_idx = 0; col_idx < 8; ++col_idx) {
            dct_matrix[row_idx][col_idx] = 0.5 * cos(((2*col_idx + 1) * row_idx * M_PI) / 16);
        }
    }

    return dct_matrix;
}


void calculate_DCT_8_by_8_block(YCbCr_Img_Matrix & original_image, std::vector<std::vector<double>> & transformed_image,
                                std::vector<std::vector<double>> & dct_matrix, int img_row_idx, int img_col_idx) 
{
    
    // 1. Make transformed_image = dct_matrix * Original Image (F)
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            double val = 0;

            for (int i = 0; i < 8; ++i) {
                val += dct_matrix[img_row_idx+row][img_col_idx+col+i] * original_image[img_row_idx+row+i][img_col_idx+col];
            }

            transformed_image[img_row_idx+row][img_col_idx+col] = val;
        }
    }

    // 2. Make transformed_image = transformed_image * DCT_Transposed
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            double val = 0;

            for (int i = 0; i < 8; ++i) {
                // Transposing image DCT_Matrix_Transpose[i][j] = DCT_Matrix[j][i]
                val += transformed_image[img_row_idx+row][img_col_idx+col+i] * dct_matrix[img_col_idx+col][img_row_idx+row+i];
            }

            transformed_image[img_row_idx+row][img_col_idx+col] = val;
        }
    }
}


// Performs DCT Operation (T = DCT * F * DCT_Transformed) (F - Original Image) (T = Transformed Image)
std::vector<std::vector<double>> perform_DCT_operation(YCbCr_Img_Matrix & original_image) {

    // Need values of YCbCr Image to go from 0-255 --> -128-127 (Subtract 128 to each element)
    for (int row_idx = 0; row_idx < original_image.size(); ++row_idx) {
        for (int col_idx = 0; col_idx < original_image[0].size(); ++col_idx) {
            original_image[row_idx][col_idx] -= 128;
        }
    }
    
    std::vector<std::vector<double>> dct_matrix = create_discrete_cosine_transform_matrix();

    std::vector<std::vector<double>> transformed_image;
    transformed_image.resize(original_image.size());

    for (int i = 0; i < original_image.size(); ++i) {
        transformed_image[i].resize(original_image[0].size());
    }

    for (int img_row_idx = 0; img_row_idx < transformed_image.size(); img_row_idx+=8) {
        for (int img_col_idx = 0; img_col_idx < transformed_image[0].size(); img_col_idx+=8) {
            calculate_DCT_8_by_8_block(original_image, transformed_image, dct_matrix, img_row_idx, img_col_idx);
        }
    }

    return transformed_image;
}