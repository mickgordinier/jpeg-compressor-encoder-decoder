#ifndef COMMON_TYPES_HPP
#define COMMON_TYPES_HPP

#include <vector>
#include <cstdint>

struct RGB_Val {
    uint8_t r, g, b;
};

struct YCbCr_Val {
    double y, cb, cr;
};

typedef std::vector<std::vector<RGB_Val>> RGB_Img_Matrix;
typedef std::vector<std::vector<YCbCr_Val>> YCbCr_Img_Matrix;

#endif // COMMON_TYPES_HPP