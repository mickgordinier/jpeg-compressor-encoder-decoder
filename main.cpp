#include "src/JpegCompression.hpp"

int main(int argc, char *argv[])
{
  /* Performing JPEG Compression and Encoding */
  /* Arguments: ./a.out <BMP_IMAGE>.bmp */
  if (argc != 3)
  {
    std::cout << "Need to input BMP file" << std::endl;
    exit(1);
  }

  /* TODO: Check type of file and confirm it ends in .bmp */

  std::string filename = argv[1];
  std::uint8_t qualityFactor = atoi(argv[2]);
  
  // Step 1: Decode uncompressed Bitmap file and obtain RGB matrix to encode
  BitmapDecoder bitmap(filename);

  #ifdef MAIN_DEBUG
    std::cout << "ORIGINAL RGB MATRIX" << std::endl;
    for (std::uint32_t rowIdx = 0; rowIdx < 8; ++rowIdx)
    {
      for (std::uint32_t colIdx = 0; colIdx < 8; ++colIdx)
      {
        std::cout << unsigned(bitmap.rgbImgMatrix[rowIdx][colIdx].r) << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;

    for (std::uint32_t rowIdx = 0; rowIdx < 8; ++rowIdx)
    {
      for (std::uint32_t colIdx = 0; colIdx < 8; ++colIdx)
      {
        std::cout << unsigned(bitmap.rgbImgMatrix[rowIdx][colIdx].g) << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;

    for (std::uint32_t rowIdx = 0; rowIdx < 8; ++rowIdx)
    {
      for (std::uint32_t colIdx = 0; colIdx < 8; ++colIdx)
      {
        std::cout << unsigned(bitmap.rgbImgMatrix[rowIdx][colIdx].b) << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  #endif

  // Step 2: Convert RGB matrix to yCbCr Matrix to extract luminance channel from chroma/color channels
  // Padding image size to multiple of 8 to have correct MCU size needed for DCT
  // Padded pixels will be ignored afterwards
  YCbCrImgMatrix convertedImage = convertRgbToPaddedYCbCr(bitmap.rgbImgMatrix);

  #ifdef MAIN_DEBUG
    std::cout << "Converted RGB to YCbCr" << std::endl;
    for (std::uint32_t rowIdx = 0; rowIdx < 8; ++rowIdx)
    {
      for (std::uint32_t colIdx = 0; colIdx < 8; ++colIdx)
      {
        std::cout << convertedImage[rowIdx][colIdx].y << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;

    for (std::uint32_t rowIdx = 0; rowIdx < 8; ++rowIdx)
    {
      for (std::uint32_t colIdx = 0; colIdx < 8; ++colIdx)
      {
        std::cout << convertedImage[rowIdx][colIdx].cb << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;

    for (std::uint32_t rowIdx = 0; rowIdx < 8; ++rowIdx)
    {
      for (std::uint32_t colIdx = 0; colIdx < 8; ++colIdx)
      {
        std::cout << convertedImage[rowIdx][colIdx].cr << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  #endif

  // Step 3: Downsampling Chroma Components TBD
  
  // Step 4: Discrete Cosine Transform (DCT)
  // Transforming 8x8 image blocks from spacial domain --> "frequency domain"
  // Will be applied to each channel
  // Performing an in-place DCT transformation on an 8x8 pixel block within the YCbCr Image
  // DCT Transformation = (DCT Matrix) * (Original Image) * (DCT Matrix Transformed)
  performDCT(convertedImage);

  #ifdef MAIN_DEBUG
    std::cout << "DCT of Image" << std::endl;
    for (std::uint32_t rowIdx = 0; rowIdx < 8; ++rowIdx)
    {
      for (std::uint32_t colIdx = 0; colIdx < 8; ++colIdx)
      {
        std::cout << convertedImage[rowIdx][colIdx].y << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;

    for (std::uint32_t rowIdx = 0; rowIdx < 8; ++rowIdx)
    {
      for (std::uint32_t colIdx = 0; colIdx < 8; ++colIdx)
      {
        std::cout << convertedImage[rowIdx][colIdx].cb << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;

    for (std::uint32_t rowIdx = 0; rowIdx < 8; ++rowIdx)
    {
      for (std::uint32_t colIdx = 0; colIdx < 8; ++colIdx)
      {
        std::cout << convertedImage[rowIdx][colIdx].cr << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  #endif

  // Step 5: Quantization
  // Removing information from the DCT image
  // Image Quality - # btw. 0 - 100 to specify how much information you would like to keep
  // The higher the quality, the higher the number of information that will stay on the image
  quantization(convertedImage, qualityFactor);

  // Step 6: Entropy Encoding
  // Performs a sequential Run-Length Encoding (RLE) on each block
  // Performs Huffman encoding
  int previousDcCoefficient = 0;
  for (std::uint32_t rowOffset = 0; rowOffset < convertedImage.size(); ++rowOffset) {
    for (std::uint32_t colOffset = 0; colOffset < convertedImage[0].size(); ++colOffset) {
      std::cout << getRunLengthEncoding(convertedImage, rowOffset, colOffset, previousDcCoefficient);
    }
  }

  return 0;
}