#include "JpegCompression.hpp"

static void transformDctBlock_8x8(YCbCrImgMatrix &imageToTransform, 
                                  const std::vector<std::vector<double>> &dctMatrix, 
                                  int rowIdx, int colIdx);
static void shiftImageValuesBack128(YCbCrImgMatrix &imageToTransform, 
                                    std::uint32_t numRows, std::uint32_t numCols);

void performDCT(YCbCrImgMatrix &imageToTransform);

/*
 * Source: https://www.opennet.ru/docs/formats/jpeg.txt
 * The purpose of the DCT transform is that instead of processing the original
 * samples, you work with the spatial frequencies present in the original image.
 * These spatial frequencies are very related to the level of detail present in an
 * image. High spatial frequencies corresponds to high levels of detail, while
 * lower frequencies corresponds to lower levels of detail.
 */

/*
Source: https://www.math.cuhk.edu.hk/~lmlui/dct.pdf
Discrete Cosine Transform (DCT) is commonly used in signal processing and image processing as a method to transform an image from the spatial domain to the frequency domain.
The idea behind DCT is that any function can be represented by the summation of cosine functions.
DCT is a reversible process and inherently lossless.
*/

// Performing an in-place DCT transformation on an 8x8 pixel block within the YCbCr Image
// DCT Transformation = (DCT Matrix) * (Original Image) * (DCT Matrix Transformed)
static void
transformDctBlock_8x8(
  YCbCrImgMatrix &imageToTransform,
  int rowOffset,
  int colOffset)
{
  // Need matrix to hold intermediate values
  YCbCrImgMatrix tempMatrix;
  tempMatrix.resize(8);
  for (int rowIdx = 0; rowIdx < 8; ++rowIdx)
  {
    tempMatrix[rowIdx].resize(8);
  }

  // 1. Make transformed_image = dctMatrix * Original Image (F)
  for (int row = 0; row < 8; ++row)
  {
    for (int col = 0; col < 8; ++col)
    {

      YCbCrVal sum_YCbCr = {0, 0, 0};

      for (int i = 0; i < 8; ++i)
      {
        sum_YCbCr.y += (dctMatrix[row][i] * imageToTransform[rowOffset + i][colOffset + col].y);
        sum_YCbCr.cb += (dctMatrix[row][i] * imageToTransform[rowOffset + i][colOffset + col].cb);
        sum_YCbCr.cr += (dctMatrix[row][i] * imageToTransform[rowOffset + i][colOffset + col].cr);
      }

      tempMatrix[row][col] = sum_YCbCr;
    }
  }

  // 2. Make transformed_image = matrix multiplication transformed_image * DCT_Transposed
  for (int row = 0; row < 8; ++row)
  {
    for (int col = 0; col < 8; ++col)
    {

      YCbCrVal sum_YCbCr = {0, 0, 0};

      for (int i = 0; i < 8; ++i)
      {
        // Transposing image DCT_Matrix_Transpose[i][j] = DCT_Matrix[j][i]
        sum_YCbCr.y += (tempMatrix[row][i].y * dctMatrix[col][i]);
        sum_YCbCr.cb += (tempMatrix[row][i].cb * dctMatrix[col][i]);
        sum_YCbCr.cr += (tempMatrix[row][i].cr * dctMatrix[col][i]);
      }

      imageToTransform[rowOffset + row][colOffset + col] = sum_YCbCr;
    }
  }
}

static void
shiftImageValuesBack128(
  YCbCrImgMatrix &imageToTransform, 
  std::uint32_t numRows, 
  std::uint32_t numCols)
{
  // Need values of YCbCr Image to go from 0-255 --> -128-127 (Subtract 128 to each element)
  for (std::uint32_t rowIdx = 0; rowIdx < numRows; ++rowIdx)
  {
    for (std::uint32_t colIdx = 0; colIdx < numCols; ++colIdx)
    {
      imageToTransform[rowIdx][colIdx].y -= 128;
      imageToTransform[rowIdx][colIdx].cb -= 128;
      imageToTransform[rowIdx][colIdx].cr -= 128;
    }
  }
}

// Performs DCT Operation
// DCT Transformation = (DCT Matrix) * (Original Image) * (DCT Matrix Transformed)
// Before computing the DCT of each 8x8 block, each value in the matrix must be recentered around zero. I.e., the midpoint must be 0.
void performDCT(
  YCbCrImgMatrix &imageToTransform)
{
  std::uint32_t numRows = imageToTransform.size();
  std::uint32_t numCols = imageToTransform[0].size();

  // Need values of YCbCr Image to go from 0-255 --> -128-127 (Subtract 128 to each element)
  // This is to have a valid midpoint of 0
  shiftImageValuesBack128(imageToTransform, numRows, numCols);

  for (std::uint32_t rowIdx = 0; rowIdx < numRows; rowIdx += 8)
  {
    for (std::uint32_t colIdx = 0; colIdx < numCols; colIdx += 8)
    {
      transformDctBlock_8x8(imageToTransform, rowIdx, colIdx);
    }
  }
}

// Performing an in-place DCT transformation on an 8x8 pixel block within the YCbCr Image
// Original Image = (DCT Matrix Transposed) * (DCT Transformation) * (DCT Matrix)
static void
transformInverseDctBlock_8x8(
  YCbCrImgMatrix &imageToTransform,
  int rowOffset,
  int colOffset)
{
  // Need matrix to hold intermediate values
  YCbCrImgMatrix tempMatrix;
  tempMatrix.resize(8);
  for (int rowIdx = 0; rowIdx < 8; ++rowIdx)
  {
    tempMatrix[rowIdx].resize(8);
  }

  // 1. Make transformed_image = dctMatrix * Original Image (F)
  for (int row = 0; row < 8; ++row)
  {
    for (int col = 0; col < 8; ++col)
    {

      YCbCrVal sum_YCbCr = {0, 0, 0};

      for (int i = 0; i < 8; ++i)
      {
        sum_YCbCr.y += (dctMatrix[i][row] * imageToTransform[rowOffset + i][colOffset + col].y);
        sum_YCbCr.cb += (dctMatrix[i][row] * imageToTransform[rowOffset + i][colOffset + col].cb);
        sum_YCbCr.cr += (dctMatrix[i][row] * imageToTransform[rowOffset + i][colOffset + col].cr);
      }

      tempMatrix[row][col] = sum_YCbCr;
    }
  }

  // 2. Make transformed_image = matrix multiplication transformed_image * DCT_Transposed
  for (int row = 0; row < 8; ++row)
  {
    for (int col = 0; col < 8; ++col)
    {

      YCbCrVal sum_YCbCr = {0, 0, 0};

      for (int i = 0; i < 8; ++i)
      {
        // Transposing image DCT_Matrix_Transpose[i][j] = DCT_Matrix[j][i]
        sum_YCbCr.y += (tempMatrix[row][i].y * dctMatrix[i][col]);
        sum_YCbCr.cb += (tempMatrix[row][i].cb * dctMatrix[i][col]);
        sum_YCbCr.cr += (tempMatrix[row][i].cr * dctMatrix[i][col]);
      }

      imageToTransform[rowOffset + row][colOffset + col] = sum_YCbCr;
    }
  }
}

static void
shiftImageValuesForward128(
  YCbCrImgMatrix &imageToTransform, 
  std::uint32_t numRows, 
  std::uint32_t numCols)
{
  for (std::uint32_t rowIdx = 0; rowIdx < numRows; ++rowIdx)
  {
    for (std::uint32_t colIdx = 0; colIdx < numCols; ++colIdx)
    {
      imageToTransform[rowIdx][colIdx].y = std::min(imageToTransform[rowIdx][colIdx].y + 128, static_cast<double>(255));
      imageToTransform[rowIdx][colIdx].cb = std::min(imageToTransform[rowIdx][colIdx].cb + 128, static_cast<double>(255));
      imageToTransform[rowIdx][colIdx].cr = std::min(imageToTransform[rowIdx][colIdx].cr + 128, static_cast<double>(255));
    }
  }
}

// Performs Inverse DCT Operation
// Compressed Image = inv(DCT Matrix) * (DCT Image) * inv(DCT Matrix Transposed)
//                  = (DCT Matrix Transposed) * (DCT Image) * (DCT Matrix)
// Before computing the DCT of each 8x8 block, each value in the matrix must be recentered around zero. I.e., the midpoint must be 0.
void 
performIDCT(
  YCbCrImgMatrix &imageToTransform)
{
  std::uint32_t numRows = imageToTransform.size();
  std::uint32_t numCols = imageToTransform[0].size();

  // Need values of YCbCr Image to go from 0-255 --> -128-127 (Subtract 128 to each element)
  // This is to have a valid midpoint of 0

  for (std::uint32_t rowIdx = 0; rowIdx < numRows; rowIdx += 8)
  {
    for (std::uint32_t colIdx = 0; colIdx < numCols; colIdx += 8)
    {
      transformInverseDctBlock_8x8(imageToTransform, rowIdx, colIdx);
    }
  }

  shiftImageValuesForward128(imageToTransform, numRows, numCols);
}

