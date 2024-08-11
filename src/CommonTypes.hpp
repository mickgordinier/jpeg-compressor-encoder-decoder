#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <vector>
#include <cstdint>

// RGB are 8-bit unsigned values when read
struct RGB_Val
{
  uint8_t r, g, b;
};

struct YCbCr_Val
{
  double y, cb, cr;
};

typedef std::vector<std::vector<RGB_Val>> RgbImgMatrix;
typedef std::vector<std::vector<YCbCr_Val>> YCbCrImgMatrix;

#endif // COMMON_TYPES_H