#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <vector>
#include <cstdint>

// RGB are 8-bit unsigned values when read
struct RgbVal
{
  std::uint8_t r, g, b;
};

struct YCbCrVal
{
  double y, cb, cr;
};

typedef std::vector<std::vector<RgbVal>> RgbImgMatrix;
typedef std::vector<std::vector<YCbCrVal>> YCbCrImgMatrix;

#endif // COMMON_TYPES_H