#ifndef JPEGCOMPRESSION_H
#define JPEGCOMPRESSION_H

#include <iostream>
#include <array>
#include <vector>
#include "CommonTypes.hpp"

/*
* Description: This files holds all of the functionality required take a Bitmap
*              image (.bmp file) and transform it to a JPEG image.
*/

/* ################# BITMAP DECODING BEGIN ################# */

class BitmapDecoder
{
private:
  void
  createRgbMatrix(
    std::ifstream &bmpFile);

public:
  BitmapDecoder(
    std::string filename);

  void
  printRgbMatrix(
    void);

  RgbImgMatrix rgbImgMatrix;
  uint32_t height;
  uint32_t width;
};

/* ################# BITMAP DECODING END ################# */

/* ################# YCBCR TRANSFORMATION BEGIN ################# */

YCbCrImgMatrix
convertRgbToPaddedYCbCr(
  const RgbImgMatrix &rgb_matrix);

/* ################# YCBCR TRANSFORMATION END ################# */

/* ################# DISCRETE COSINE TRANFORMATION BEGIN ################# */

std::vector<std::vector<double>>
create_discrete_cosine_transform_matrix(
  void);

void
transform_DCT_8_by_8_block(
  YCbCrImgMatrix &image_to_transform,
  const std::vector<std::vector<double>> &dct_matrix,
  int img_row_idx,
  int img_col_idx);

void
perform_DCT_operation(
  YCbCrImgMatrix &image_to_transform);

/* ################# DISCRETE COSINE TRANFORMATION END ################# */

/* ################# QUANTIZATION BEGIN ################# */

const std::vector<std::vector<double>> LUMINANCE_QUANTIZATION_MATRIX = {
  {16, 11, 10, 16, 24, 40, 51, 61},
  {12, 12, 14, 19, 26, 58, 60, 55},
  {14, 13, 16, 24, 40, 57, 69, 56},
  {14, 17, 22, 29, 51, 87, 80, 62},
  {18, 22, 37, 56, 68, 109, 103, 77},
  {24, 35, 55, 64, 81, 103, 113, 92},
  {49, 63, 78, 87, 103, 121, 120, 101},
  {72, 92, 95, 98, 112, 100, 103, 99}};

const std::vector<std::vector<double>> CHROMA_QUANTIZATION_MATRIX = {
  {17, 18, 24, 47, 99, 99, 99, 99},
  {18, 21, 26, 66, 99, 99, 99, 99},
  {24, 26, 56, 99, 99, 99, 99, 99},
  {47, 66, 99, 99, 99, 99, 99, 99},
  {99, 99, 99, 99, 99, 99, 99, 99},
  {99, 99, 99, 99, 99, 99, 99, 99},
  {99, 99, 99, 99, 99, 99, 99, 99},
  {99, 99, 99, 99, 99, 99, 99, 99}};

void quantization(
  YCbCrImgMatrix &transformed_dct_image,
  uint8_t image_quality);

/* ################# QUANTIZATION END ################# */

#endif