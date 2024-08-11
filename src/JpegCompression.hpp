#ifndef JPEGCOMPRESSION_H
#define JPEGCOMPRESSION_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <array>
#include <vector>
#include <cmath>
#include <math.h>
#include <cstdint>
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
  
  std::uint32_t fileSizeBytes;
  std::uint32_t reservedBytes;
  std::uint32_t imageSize;
  std::uint32_t xResolution;
  std::uint32_t yResolution;

public:
  BitmapDecoder(
    std::string filename);

  void
  printRgbMatrix(
    void);
  
  void
  createOutputFile(
    std::string outFilename,
    RgbImgMatrix &newImageMatrix);

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

// Instead of recomputing the dct Matrix each time, decided it's best to create it
// Additionally, as the DCT Matrix is orthogonal, inverse(dctMatrix) = transpose(dctMatrix)
// Source: https://dev.to/marycheung021213/understanding-dct-and-quantization-in-jpeg-compression-1col#:~:text=Discrete%20Cosine%20Transform%20(DCT)%20is,the%20summation%20of%20cosine%20functions.
const std::vector<std::vector<double>> dctMatrix = {
 {0.3535533,  0.3535533,  0.3535533,  0.3535533,  0.3535533,  0.3535533,  0.3535533,  0.3535533},
 {0.4903926,  0.4157348,  0.2777851,  0.0975451, -0.0975451, -0.2777851, -0.4157348, -0.4903926},
 {0.4619397,  0.1913417, -0.1913417, -0.4619397, -0.4619397, -0.1913417,  0.1913417,  0.4619397},
 {0.4157348, -0.0975451, -0.4903926, -0.2777851,  0.2777851,  0.4903926,  0.0975451, -0.4157348},
 {0.3535533, -0.3535533, -0.3535533,  0.3535533,  0.3535533, -0.3535533, -0.3535533,  0.3535533},
 {0.2777851, -0.4903926,  0.0975451,  0.4157348, -0.4157348, -0.0975451,  0.4903926, -0.2777851},
 {0.1913417, -0.4619397,  0.4619397, -0.1913417, -0.1913417,  0.4619397, -0.4619397,  0.1913417},
 {0.0975451, -0.2777851,  0.4157348, -0.4903926,  0.4903926, -0.4157348,  0.2777851, -0.0975451}
};

void
performDCT(
  YCbCrImgMatrix &imageToTransform);

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
  YCbCrImgMatrix &dctImage,
  uint8_t image_quality);

/* ################# QUANTIZATION END ################# */

#endif