#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <cstdint>
#include <cstring>
#include "JpegCompression.hpp"

#define HEADER_SIGNATURE     "BM"
#define OFFSET_TO_PIXEL_DATA 54   // (14 Byte Header + 40 byte InfoHeader)
#define INFOHEADER_SIZE      40   // Size of BITMAPINFOHEADER
#define NUM_COLOR_PLANES     1
#define BITS_PER_PIXELS      24
#define COMPRESSION_TYPE     0
#define NUM_COLORS           0
#define NUM_IMPORTANT_COLORS 0

enum ColorLayer
{
  RED = 0,
  GREEN,
  BLUE
};

static std::uint32_t getWord(std::ifstream &bmp_file);
static std::uint32_t getHalfword(std::ifstream &bmp_file);
static void          skipBytes(std::ifstream &bmp_file, std::uint8_t bytes_to_skip);
static const RgbVal getBitmapRgb(std::ifstream &bmpFile);

#ifdef BITMAP_DEBUG
  static void
  bitmapDebugPrint(
    const std::string &variableName,
    const std::uint32_t value)
  {
    std::cout << variableName << ": " << value << std::endl;
  }
#endif

// return nexts 4 bytes as little endian uint32_t
static std::uint32_t
getWord(
  std::ifstream &bmp_file)
{
  const std::uint32_t word = (bmp_file.get()) +
                             (bmp_file.get() << 8) +
                             (bmp_file.get() << 16) +
                             (bmp_file.get() << 24);
  return word;
}

// return nexts 2 bytes as little endian uint32_t
static std::uint32_t
getHalfword(
  std::ifstream &bmp_file)
{
  const std::uint32_t word = (bmp_file.get()) +
                             (bmp_file.get() << 8);
  return word;
}

static void
skipBytes(
  std::ifstream &bmp_file,
  std::uint8_t bytes_to_skip)
{
  for (uint8_t i = 0; i < bytes_to_skip; ++i)
  {
    bmp_file.get();
  }
}

/*
* Reads in the Bitmap image and gathers the important information.
*/
BitmapDecoder::BitmapDecoder(
  std::string filename)
{
  // opening up bmp img bytes to read
  // setting i/o stream base mode to binary
  std::ifstream bmp_file(filename, std::ios_base::binary);

  if (!bmp_file) {
    std::cout << "ERROR: File stream (" << filename << ") not able to be opened" << std::endl;
    exit(-1);
  }

  // READING IN BITMAP HEADER AND INFOHEADER
  /* NOTE: There exist different infoheader versions (~8 different versions).
  *        We will be condensing all future Bitmap versions into the version
  *        descripbed as BITMAPINFOHEADER. All future versions are extensions
  *        More information about the different versions of infoheaders 
  *        can be found here: https://en.wikipedia.org/wiki/BMP_file_format
  */

  /* Possible Header Signatures:
  * BM - Windows 3.1x, 95, NT, ... etc. (!!!)
  * BS - OS/2 struct bitmap array       (Outdated)
  * CI - OS/2 struct color icon         (Outdated)
  * CP - OS/2 const color pointer       (Outdated)
  * IC - OS/2 struct icon               (Outdated)
  * PT - OS/2 pointer                   (Outdated)
  */
  const char headerSignature[3] = {(char)bmp_file.get(),  (char)bmp_file.get(), '\0'};
  if (strcmp(headerSignature, "BM") != 0) {
    std::cout << "ERROR: The provided bitmap is too early of a version to be decoded" << std::endl;
    std::cout << "ERROR: The OS/2 version of this bitmap is no longer supported after Windows 2000" << std::endl;
    exit(-1);
  }

  fileSizeBytes = getWord(bmp_file);

  reservedBytes = getWord(bmp_file);

  #ifdef BITMAP_DEBUG
    std::cout << std::endl << "########################################" << std::endl << std::endl;
    std::cout << "DEBUG: Reading in Bitmap information" << std::endl <<std::endl;

    std::cout << "Bitmap Header Information" << std::endl;
    std::cout << "Header Signature: " << headerSignature << std::endl;
    bitmapDebugPrint("File Size (Bytes)", fileSizeBytes);
    bitmapDebugPrint("Reserved Bytes Value", reservedBytes);
    bitmapDebugPrint("Offset to Pixel Data (Bytes)", getWord(bmp_file));
  #else
    // ignore next 4 bytes  (offset from beginning of file to beginning of bitmap data)
    skipBytes(bmp_file, 4);
  #endif

  /* The size of the InfoHeader determine what version we are dealing with*/
  std::uint32_t infoHeaderSize = getWord(bmp_file);

  #ifdef BITMAP_DEBUG
    std::cout << std::endl << "Bitmap InfoHeader Information" << std::endl;
    bitmapDebugPrint("Size of InfoHeader (Bytes)", infoHeaderSize);
  #endif

  std::string infoHeaderVersion;
  if (infoHeaderSize == 40) {
    infoHeaderVersion = "BITMAPINFOHEADER";
  } else if (infoHeaderSize == 52) {
    infoHeaderVersion = "BITMAPV2INFOHEADER";
  } else if (infoHeaderSize == 56) {
    infoHeaderVersion = "BITMAPV3INFOHEADER";
  } else if (infoHeaderSize == 108) {
    infoHeaderVersion = "BITMAPV4HEADER";
  } else if (infoHeaderSize == 124) {
    infoHeaderVersion = "BITMAPV5HEADER";
  }  else {
    std::cout << "ERROR: INVALID InfoHeader Size" << std::endl;
    exit(-1);
  }

  // getting horizontal width of Bitmap in pixels
  width = getWord(bmp_file);

  // getting horizontal height of Bitmap in pixels
  height = getWord(bmp_file);

  // Number of color planes need to be 1
  std::uint32_t numColorPlanes = getHalfword(bmp_file);

  /* There are many types of Pixel Formats (in bits per pixel).
  *  Below are the Pixel formats allowed by this extractor, which are the most common:
  *  24 bbp - (3 bytes == 24 bits)/pixel. {8-bit blue, 8-bit green, 8-bit red} in that order
  */
  uint32_t bitsPerPixel = getHalfword(bmp_file);

  /* This extractor only works with uncompressed Bitmap images
  *  Bitmaps can be encoded using RLE and Huffman Encoding.
  *  Thus, this Bitmap image MUST have a compression type of 0.
  */
  uint32_t compressionType = getWord(bmp_file);

  #ifdef BITMAP_DEBUG
    std::cout << "InfoHeader Version Name: " << infoHeaderVersion << std::endl;
    bitmapDebugPrint("Image Width (Bytes)", width);
    bitmapDebugPrint("Image Height (Bytes)", height);
    bitmapDebugPrint("Number of Color Planes", numColorPlanes);
    bitmapDebugPrint("Bits per Pixel", bitsPerPixel);
    bitmapDebugPrint("Compression Type", compressionType);
  #endif

  if (numColorPlanes != 1) {
    std::cout << "ERROR: The provided bitmap has an invalid number of color planes" << std::endl;
    std::cout << "ERROR: The number of color planes must be = 1." << std::endl;
    exit(-1);
  }

  if (bitsPerPixel != 24) {
    std::cout << "ERROR: This extractor only works with 24 bits per pixel images" << std::endl;
    exit(-1);
  }

  if (compressionType != 0) {
    std::cout << "ERROR: This extractor cannot work with compressed Bitmap images" << std::endl;
    exit(-1);
  }

  imageSize = getWord(bmp_file);
  xResolution = getWord(bmp_file);
  yResolution = getWord(bmp_file);

  #ifdef BITMAP_DEBUG
    bitmapDebugPrint("Image Size (Bytes)", imageSize);
    bitmapDebugPrint("X-resolution (Pixel per Meter)", xResolution);
    bitmapDebugPrint("Y-resolution (Pixel per Meter)", yResolution);
    bitmapDebugPrint("Number of Colors", getWord(bmp_file));
  #else
    // ignore next 4 bytes (colors used either 0 or 2^n)
    skipBytes(bmp_file, 4);
  #endif

  uint32_t numImportantColors = getWord(bmp_file);

  #ifdef BITMAP_DEBUG
    bitmapDebugPrint("Number of Important Colors", numImportantColors);
  #endif

  if (numImportantColors != 0) {
    std::cout << "ERROR: This extractor cannot work with distinct color options" << std::endl;
    exit(-1);
  }

  #ifdef BITMAP_DEBUG
    std::cout << std::endl << "########################################" << std::endl << std::endl;
  #endif

  skipBytes(bmp_file, infoHeaderSize - INFOHEADER_SIZE);

  // creating 3D vector of size height x width x 3 layers for RGB
  BitmapDecoder::createRgbMatrix(bmp_file);
}

/* Bytes are read in as {8-bit blue, 8-bit green, 8-bit red} in 24-bit rgb */
static const RgbVal
getBitmapRgb(
  std::ifstream &bmpFile)
{
  RgbVal rgbVal;
  rgbVal.b = bmpFile.get();
  rgbVal.g = bmpFile.get();
  rgbVal.r = bmpFile.get();

  return rgbVal;
}

void
BitmapDecoder::createRgbMatrix(
  std::ifstream &bmp_file)
{

  // reserving space for RGB Matrix
  rgbImgMatrix.resize(height);
  for (uint32_t rowIdx = 0; rowIdx < height; ++rowIdx)
  {
    rgbImgMatrix[rowIdx].resize(width);
  }

  /*
  *  Source: https://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm
  *  Each scan line is zero padded to the nearest 4-byte boundary. If the image has a width that is not divisible by four, say, 21 bytes,
  *  there would be 3 bytes of padding at the end of every scan line.
  */
  std::uint8_t num_padding_bytes = 0;
  if ((width * 3) % 4 != 0)
  {
    num_padding_bytes = 4 - (width * 3) % 4;
  }

  // image read goes bottom-up
  for (std::uint32_t rowIdx = height; rowIdx > 0; --rowIdx)
  {
    // image read goes from left to right
    for (std::uint32_t colIdx = 0; colIdx < width; ++colIdx)
    {
      rgbImgMatrix[rowIdx - 1][colIdx] = getBitmapRgb(bmp_file);
    }

    // accounting for padding bytes in file
    skipBytes(bmp_file, num_padding_bytes);
  }
}

// Prints similar to format as numpy matrix on python
void
BitmapDecoder::printRgbMatrix(
  void)
{

  for (std::uint8_t colorLayer = RED; colorLayer <= BLUE; ++colorLayer)
  {
    std::cout << "[";
    for (std::uint32_t rowIdx = 0; rowIdx < rgbImgMatrix.size(); ++rowIdx)
    {
      if (rowIdx != 0)
      {
        std::cout << " ";
      }

      std::cout << "[";

      for (std::uint32_t colIdx = 0; colIdx < rgbImgMatrix[0].size(); ++colIdx)
      {

        uint8_t value;
        if (colorLayer == RED)
        {
          value = unsigned(rgbImgMatrix[rowIdx][colIdx].r);
        }
        else if (colorLayer == GREEN)
        {
          value = unsigned(rgbImgMatrix[rowIdx][colIdx].g);
        }
        else
        {
          value = unsigned(rgbImgMatrix[rowIdx][colIdx].b);
        }

        uint8_t numspaces = (value / 100 <= 0) + (value / 10 <= 0);
        for (uint8_t i = 0; i < numspaces; ++i)
        {
          std::cout << " ";
        }

        std::cout << unsigned(value);

        if (colIdx < rgbImgMatrix[0].size() - 1)
        {
          std::cout << ", ";
        }
      }
      std::cout << "]";

      if (rowIdx < rgbImgMatrix.size() - 1)
      {
        std::cout << "," << std::endl;
      }
    }

    std::cout << "]" << std::endl << std::endl;
  }
  std::cout << std::endl;
}

static void
putNBytes(
  std::ofstream &outFileStream,
  std::uint32_t numberToWrite,
  std::uint8_t numBytes)
{
  for (uint8_t byteIdx = 0; byteIdx < numBytes; ++byteIdx) {
    outFileStream.put(numberToWrite & 0xFF);
    numberToWrite >>= 8;
  }
}

void
BitmapDecoder::createOutputFile(
  std::string outFilename,
  RgbImgMatrix &newImageMatrix)
{
  std::ofstream outFileStream(outFilename, std::ios_base::binary);

  if (!outFileStream) {
    std::cout << "ERROR cannot open output file stream" << std::endl;
    exit(-1);
  }

  outFileStream << HEADER_SIGNATURE;

  putNBytes(outFileStream, fileSizeBytes, 4);
  putNBytes(outFileStream, reservedBytes, 4);
  putNBytes(outFileStream, OFFSET_TO_PIXEL_DATA, 4);

  putNBytes(outFileStream, INFOHEADER_SIZE, 4);
  putNBytes(outFileStream, width, 4);
  putNBytes(outFileStream, height, 4);
  putNBytes(outFileStream, NUM_COLOR_PLANES, 2);
  putNBytes(outFileStream, BITS_PER_PIXELS, 2);
  putNBytes(outFileStream, COMPRESSION_TYPE, 4);
  putNBytes(outFileStream, imageSize, 4);
  putNBytes(outFileStream, xResolution, 4);
  putNBytes(outFileStream, yResolution, 4);
  putNBytes(outFileStream, NUM_COLORS, 4);
  putNBytes(outFileStream, NUM_IMPORTANT_COLORS, 4);

  for (std::uint32_t rowIdx = height; rowIdx > 0; --rowIdx) {
    for (std::uint32_t colIdx = 0; colIdx < width; ++colIdx) {
      outFileStream.put(newImageMatrix[rowIdx-1][colIdx].b);
      outFileStream.put(newImageMatrix[rowIdx-1][colIdx].g);
      outFileStream.put(newImageMatrix[rowIdx-1][colIdx].r);
    }
  }
}