#ifndef DISCRETE_COSINE_TRANSFORMATION_H
#define DISCRETE_COSINE_TRANSFORMATION_H

#include "CommonTypes.hpp"

/*
* Source: https://www.math.cuhk.edu.hk/~lmlui/dct.pdf
* Discrete Cosine Transform (DCT) is commonly used in signal processing and image processing as a method to transform an image from the spatial domain to the frequency domain. 
* The idea behind DCT is that any function can be represented by the summation of cosine functions. 
* DCT is a reversible process and inherently lossless.
*/

/*
* Source: https://www.opennet.ru/docs/formats/jpeg.txt
* The purpose of the DCT transform is that instead of processing the original
* samples, you work with the spatial frequencies present in the original image.
* These spatial frequencies are very related to the level of detail present in an
* image. High spatial frequencies corresponds to high levels of detail, while
* lower frequencies corresponds to lower levels of detail.
*/

// creates the 8x8 DCT Transformation Matrix
// Going from "time domain" --> "frequency domain" in which you get 64 coefficients which are the frequencies' amplitudes
// Source: https://dev.to/marycheung021213/understanding-dct-and-quantization-in-jpeg-compression-1col#:~:text=Discrete%20Cosine%20Transform%20(DCT)%20is,the%20summation%20of%20cosine%20functions.
std::vector<std::vector<double>> create_discrete_cosine_transform_matrix();

// Performing an in-place DCT transformation on an 8x8 pixel block within the YCbCr Image
// NOTE: Each DCT is applied individually to each channel of the 8x8 block
// DCT Transformation = (DCT Matrix) * (Original Image) * (DCT Matrix Transformed)
void transform_DCT_8_by_8_block(YCbCr_Img_Matrix & image_to_transform, 
                                const std::vector<std::vector<double>> & dct_matrix, 
                                int img_row_idx, int img_col_idx);

// Performs DCT Operation
// DCT Transformation = (DCT Matrix) * (Original Image) * (DCT Matrix Transformed)
// Before computing the DCT of each 8x8 block, each value in the matrix must be recentered around zero. I.e., the midpoint must be 0.
void perform_DCT_operation(YCbCr_Img_Matrix & image_to_transform);

#endif //DISCRETE_COSINE_TRANSFORMATION_H