#include "src/JpegCompression.hpp"

/*
* DESCRIPTION: Takes in a bitmap image and produces a compressed version
*              of the bitmap.
*              ./compressBitmap <BMP_IMAGE>.bmp <compress quality>
*/

int main(int argc, char *argv[])
{
  /* Performing JPEG Compression and Encoding */
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
      for (std::uint32_t colIdx = bitmap.width-8; colIdx < bitmap.width; ++colIdx)
      {
        std::cout << unsigned(bitmap.rgbImgMatrix[rowIdx][colIdx].r) << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;

    for (std::uint32_t rowIdx = 0; rowIdx < 8; ++rowIdx)
    {
      for (std::uint32_t colIdx = bitmap.width-8; colIdx < bitmap.width; ++colIdx)
      {
        std::cout << unsigned(bitmap.rgbImgMatrix[rowIdx][colIdx].g) << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;

    for (std::uint32_t rowIdx = 0; rowIdx < 8; ++rowIdx)
    {
      for (std::uint32_t colIdx = bitmap.width-8; colIdx < bitmap.width; ++colIdx)
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
      for (std::uint32_t colIdx = bitmap.width-8; colIdx < bitmap.width; ++colIdx)
      {
        std::cout << convertedImage[rowIdx][colIdx].y << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;

    for (std::uint32_t rowIdx = 0; rowIdx < 8; ++rowIdx)
    {
      for (std::uint32_t colIdx = bitmap.width-8; colIdx < bitmap.width; ++colIdx)
      {
        std::cout << convertedImage[rowIdx][colIdx].cb << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;

    for (std::uint32_t rowIdx = 0; rowIdx < 8; ++rowIdx)
    {
      for (std::uint32_t colIdx = bitmap.width-8; colIdx < bitmap.width; ++colIdx)
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
      for (std::uint32_t colIdx = bitmap.width-8; colIdx < bitmap.width; ++colIdx)
      {
        std::cout << convertedImage[rowIdx][colIdx].y << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;

    for (std::uint32_t rowIdx = 0; rowIdx < 8; ++rowIdx)
    {
      for (std::uint32_t colIdx = bitmap.width-8; colIdx < bitmap.width; ++colIdx)
      {
        std::cout << convertedImage[rowIdx][colIdx].cb << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;

    for (std::uint32_t rowIdx = 0; rowIdx < 8; ++rowIdx)
    {
      for (std::uint32_t colIdx = bitmap.width-8; colIdx < bitmap.width; ++colIdx)
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

  #ifdef MAIN_DEBUG
    std::cout << "DCT Quantized of Image" << std::endl;
    for (std::uint32_t rowIdx = 0; rowIdx < 8; ++rowIdx)
    {
      for (std::uint32_t colIdx = bitmap.width-8; colIdx < bitmap.width; ++colIdx)
      {
        std::cout << convertedImage[rowIdx][colIdx].y << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;

    for (std::uint32_t rowIdx = 0; rowIdx < 8; ++rowIdx)
    {
      for (std::uint32_t colIdx = bitmap.width-8; colIdx < bitmap.width; ++colIdx)
      {
        std::cout << convertedImage[rowIdx][colIdx].cb << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;

    for (std::uint32_t rowIdx = 0; rowIdx < 8; ++rowIdx)
    {
      for (std::uint32_t colIdx = bitmap.width-8; colIdx < bitmap.width; ++colIdx)
      {
        std::cout << convertedImage[rowIdx][colIdx].cr << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  #endif

  // Step 6: Decompression / Dequantization
  decompressImage(convertedImage, qualityFactor);

  #ifdef MAIN_DEBUG
    std::cout << "Decompressed DCT of Image" << std::endl;
    for (std::uint32_t rowIdx = 0; rowIdx < 8; ++rowIdx)
    {
      for (std::uint32_t colIdx = bitmap.width-8; colIdx < bitmap.width; ++colIdx)
      {
        std::cout << convertedImage[rowIdx][colIdx].y << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;

    for (std::uint32_t rowIdx = 0; rowIdx < 8; ++rowIdx)
    {
      for (std::uint32_t colIdx = bitmap.width-8; colIdx < bitmap.width; ++colIdx)
      {
        std::cout << convertedImage[rowIdx][colIdx].cb << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;

    for (std::uint32_t rowIdx = 0; rowIdx < 8; ++rowIdx)
    {
      for (std::uint32_t colIdx = bitmap.width-8; colIdx < bitmap.width; ++colIdx)
      {
        std::cout << convertedImage[rowIdx][colIdx].cr << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  #endif

  // Step 6: Performing Inverse Discrete Cosine Transformation
  // Will obtain compressed YCbCr Image from DCT Image
  performIDCT(convertedImage);

  #ifdef MAIN_DEBUG
    std::cout << "IDCT of Decompressed Image" << std::endl;
    for (std::uint32_t rowIdx = 0; rowIdx < 8; ++rowIdx)
    {
      for (std::uint32_t colIdx = bitmap.width-8; colIdx < bitmap.width; ++colIdx)
      {
        std::cout << convertedImage[rowIdx][colIdx].y << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;

    for (std::uint32_t rowIdx = 0; rowIdx < 8; ++rowIdx)
    {
      for (std::uint32_t colIdx = bitmap.width-8; colIdx < bitmap.width; ++colIdx)
      {
        std::cout << convertedImage[rowIdx][colIdx].cb << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;

    for (std::uint32_t rowIdx = 0; rowIdx < 8; ++rowIdx)
    {
      for (std::uint32_t colIdx = bitmap.width-8; colIdx < bitmap.width; ++colIdx)
      {
        std::cout << convertedImage[rowIdx][colIdx].cr << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  #endif


  RgbImgMatrix compressedRgbImage = convertPaddedYCbCrtoRgb(convertedImage, bitmap.height, bitmap.width);

  #ifdef MAIN_DEBUG
    std::cout << "NEW RGB MATRIX" << std::endl;
    for (std::uint32_t rowIdx = 0; rowIdx < 8; ++rowIdx)
    {
      for (std::uint32_t colIdx = bitmap.width-8; colIdx < bitmap.width; ++colIdx)
      {
        std::cout << unsigned(compressedRgbImage[rowIdx][colIdx].r) << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;

    for (std::uint32_t rowIdx = 0; rowIdx < 8; ++rowIdx)
    {
      for (std::uint32_t colIdx = bitmap.width-8; colIdx < bitmap.width; ++colIdx)
      {
        std::cout << unsigned(compressedRgbImage[rowIdx][colIdx].g) << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;

    for (std::uint32_t rowIdx = 0; rowIdx < 8; ++rowIdx)
    {
      for (std::uint32_t colIdx = bitmap.width-8; colIdx < bitmap.width; ++colIdx)
      {
        std::cout << unsigned(compressedRgbImage[rowIdx][colIdx].b) << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  #endif

  bitmap.createOutputFile("compressedImage.bmp", compressedRgbImage);

  return 0;
}