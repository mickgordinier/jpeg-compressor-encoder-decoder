#include <vector>
#include <array>
#include <cstdint>

typedef std::array<uint8_t, 3> RGB_Val;                  // Can be uint_8 as values range from 0-255
typedef std::vector<std::vector<RGB_Val>> RGB_Img_Matrix;

typedef std::array<int16_t, 3> YCbCr_Val;               // Needs to be int16_t as values will be shifted down 128 in DCT
typedef std::vector<std::vector<YCbCr_Val>> YCbCr_Img_Matrix;

void pad_YCbCr(YCbCr_Img_Matrix & yCbCr, uint32_t original_height, uint32_t original_width);
YCbCr_Img_Matrix convert_RGB_to_padded_YCbCr(const RGB_Img_Matrix & rgb_matrix);