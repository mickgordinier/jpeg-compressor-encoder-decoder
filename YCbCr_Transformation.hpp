#ifndef YCBCR_TRANSFORMATION_HPP
#define YCBCR_TRANSFORMATION_HPP

#include "CommonTypes.hpp"
#include <vector>
#include <array>
#include <cstdint>

void pad_YCbCr(YCbCr_Img_Matrix & yCbCr, uint32_t original_height, uint32_t original_width);

YCbCr_Img_Matrix convert_RGB_to_padded_YCbCr(const RGB_Img_Matrix & rgb_matrix);

#endif YCBCR_TRANSFORMATION_HPP