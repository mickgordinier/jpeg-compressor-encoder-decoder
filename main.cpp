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

  // Step 1: Decode uncompressed Bitmap file and obtain RGB matrix to encode
  std::string filename = argv[1];
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

  // Ensuring Bitmap Conversion works
  // bitmap.createOutputFile("testOut1.bmp", bitmap.rgbImgMatrix);

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

  // Ensuring YCbCr Conversion works
  // RgbImgMatrix convertedBack = convertPaddedYCbCrtoRgb(convertedImage, bitmap.height, bitmap.width);
  // bitmap.createOutputFile("testOut2.bmp", convertedBack);

  // Step 3: Downsampling Chroma Components TBD

  std::uint8_t qualityFactor = atoi(argv[2]);

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

  // performIDCT(convertedImage);
  // RgbImgMatrix convertedBack2 = convertPaddedYCbCrtoRgb(convertedImage, bitmap.height, bitmap.width);
  // bitmap.createOutputFile("testOut3.bmp", convertedBack2);

  // Step 5: Quantization
  // Removing information from the DCT image
  // Image Quality - # btw. 0 - 100 to specify how much information you would like to keep
  // The higher the quality, the higher the number of information that will stay on the image
  quantization(convertedImage, qualityFactor);

  #ifdef MAIN_DEBUG
    std::cout << "DCT Quantized of Image" << std::endl;
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

  decompressImage(convertedImage, qualityFactor);

  #ifdef MAIN_DEBUG
    std::cout << "Decompressed DCT of Image" << std::endl;
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

  performIDCT(convertedImage);

  #ifdef MAIN_DEBUG
    std::cout << "IDCT of Decompressed Image" << std::endl;
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

  RgbImgMatrix convertedBack3 = convertPaddedYCbCrtoRgb(convertedImage, bitmap.height, bitmap.width);

  #ifdef MAIN_DEBUG
    std::cout << "NEW RGB MATRIX" << std::endl;
    for (std::uint32_t rowIdx = 0; rowIdx < 8; ++rowIdx)
    {
      for (std::uint32_t colIdx = 0; colIdx < 8; ++colIdx)
      {
        std::cout << unsigned(convertedBack3[rowIdx][colIdx].r) << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;

    for (std::uint32_t rowIdx = 0; rowIdx < 8; ++rowIdx)
    {
      for (std::uint32_t colIdx = 0; colIdx < 8; ++colIdx)
      {
        std::cout << unsigned(convertedBack3[rowIdx][colIdx].g) << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;

    for (std::uint32_t rowIdx = 0; rowIdx < 8; ++rowIdx)
    {
      for (std::uint32_t colIdx = 0; colIdx < 8; ++colIdx)
      {
        std::cout << unsigned(convertedBack3[rowIdx][colIdx].b) << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  #endif

  bitmap.createOutputFile("testOut4.bmp", convertedBack3);

  // Step 6: Entropy Encoding

  return 0;
}