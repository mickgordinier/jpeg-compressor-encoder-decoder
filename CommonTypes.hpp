#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <vector>
#include <cstdint>

// RGB are 8-bit unsigned values when read
struct RGB_Val {
    uint8_t r, g, b;
};

struct YCbCr_Val {
    double y, cb, cr;
};

typedef std::vector<std::vector<RGB_Val>> RGB_Img_Matrix;
typedef std::vector<std::vector<YCbCr_Val>> YCbCr_Img_Matrix;

#endif // COMMON_TYPES_H