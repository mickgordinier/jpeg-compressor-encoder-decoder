#include "JpegCompression.hpp"

YCbCrImgMatrix convertRgbToPaddedYCbCr(const RgbImgMatrix &rgbMatrix);
static void padYCbCr(YCbCrImgMatrix &yCbCr, uint32_t originalHeight, uint32_t originalWidth);

// Converting RGB image to YCbCr Image. Also padding image to make dimesions multiple of 8.
/*
  Source: https://www.thewebmaster.com/jpeg-definitive-guide/

  The human eye is much more sensitive to fine variations in brightness (luminance) than to changes in color (chroma).
  JPEG can take advantage of this by converting to the YCbCr color model, which splits the luminance from the chroma.
  We can then apply stronger compression to the chroma channels with less effect on image quality.

  The YCbCr color model converts from the RGB color model and consists of 3 separate categories:

  Luminance (Y) - The luminance data from each red, green, and blue RGB channel is extracted and separated from the chroma data and combined to make the Luminance (Y) channel.
                  The Y channel, by itself, contains enough data to create a complete black-and-white image.
  Chroma Blue (Cb) - The Chroma Blue Channel is the RGB blue channel minus the luminance.
  Chroma Red (Cr) - The Chroma Red Channel is the RGB blue channel minus the luminance.
  After subtracting the Y, Cb, and Cb channels from the image, the remainder provides the chroma green information. This can be mathematically extracted instead of encoded, saving bandwidth.
*/
YCbCrImgMatrix
convertRgbToPaddedYCbCr(
  const RgbImgMatrix &rgbMatrix)
{
  YCbCrImgMatrix yCbCrMatrix;

  // NEED HEIGHT AND WIDTH TO BE MULTIPLE OF 8 TO PROPERLY PERFORM DCT
  // WILL IGNORE PADDED PIXELS AFTERWARDS
  uint32_t yCbCrHeight = rgbMatrix.size();
  uint32_t yCbCrWidth = rgbMatrix[0].size();

  if (yCbCrHeight % 8 != 0)
  {
    yCbCrHeight += (8 - yCbCrHeight % 8);
  }

  if (yCbCrWidth % 8 != 0)
  {
    yCbCrWidth += (8 - yCbCrWidth % 8);
  }

  // Resizing yCbCr matrix
  yCbCrMatrix.resize(yCbCrHeight);

  for (uint32_t rowIdx = 0; rowIdx < yCbCrHeight; ++rowIdx)
  {
    yCbCrMatrix[rowIdx].resize(yCbCrWidth);
  }

  // Converting all rgb values to yCbCr values
  /*
    Source: https://www.w3.org/Graphics/JPEG/jfif.pdf
    "The color space to be used is YCbCr as defined by CCIR 601 (256 levels). The RGB
    components calculated by linear conversion from YCbCr shall not be gamma corrected
    (gamma = 1.0). If only one component is used, that component shall be Y."

    RGB to YCbCr Conversion
    YCbCr (256 levels) can be computed directly from 8-bit RGB as follows:

    Y  =  (0.299   * R) + (0.587  * G) + (0.114  * B)
    Cb =  (-0.1687 * R) - (0.3313 * G) + (0.5    * B) + 128
    Cr =  (0.5     * R) - (0.4187 * G) - (0.0813 * B) + 128
  */

  for (uint32_t rowIdx = 0; rowIdx < rgbMatrix.size(); ++rowIdx)
  {
    for (uint32_t colIdx = 0; colIdx < rgbMatrix[0].size(); ++colIdx)
    {

      RGB_Val rgbVal = rgbMatrix[rowIdx][colIdx];

      YCbCr_Val yCbCrVal;

      // luminance (Y) channel (AKA Intensity of RGB Color)
      yCbCrVal.y = (static_cast<double>(rgbVal.r) * 0.299) + (static_cast<double>(rgbVal.g) * 0.587) + (static_cast<double>(rgbVal.b) * 0.114);

      // chroma blue (Cb) channel
      yCbCrVal.cb = (static_cast<double>(rgbVal.r) * -0.1687) + (static_cast<double>(rgbVal.g) * -0.3313) + (static_cast<double>(rgbVal.b) * 0.5) + 128;

      // chroma red (Cr) channel
      yCbCrVal.cr = (static_cast<double>(rgbVal.r) * 0.5) + (static_cast<double>(rgbVal.g) * -0.4187) + (static_cast<double>(rgbVal.b) * -0.0813) + 128;

      yCbCrMatrix[rowIdx][colIdx] = yCbCrVal;
    }
  }

  padYCbCr(yCbCrMatrix, rgbMatrix.size(), rgbMatrix[0].size());

  return yCbCrMatrix;
}

// Padding with repeated pixels technique to make image size multiple of 8
static void 
padYCbCr(
  YCbCrImgMatrix &yCbCr,
  uint32_t originalHeight,
  uint32_t originalWidth)
{

  /*
  Source: https://www.thewebmaster.com/jpeg-definitive-guide/

  Block splitting is the process of splitting each channel in a source image into pixel blocks called Minimum Coded Units (MCU).
  Each channel may have a different-sized MCU, depending on whether that channel was subsampled.

  For example, here is the MCU size for the following subsampling ratios:
  4:4:4 - 8 x 8 pixels
  4:2:2 - 16 * 8 pixels
  4:2:0 - 16 * 16 pixels

  For JPEG compression algorithms to work, images must only contain complete MCU blocks.
  In other words, the image dimensions must be multiples of an MCU, for example, 8 pixels for the 4:4:4 subsampling ratio.
  For example, an image with a size of 501 x 375 pixels must first be changed to an image with a size of 504 x 376 pixels.

  If the image has partial MCU blocks, then the software encoding the JPEG image must add its own dummy data to those blocks.

  The data (or padding) added to the image depends on the software, but the following techniques may be used:
  1. Use a fixed color, such as black.
  !!!!!!!!!  2. Repeat the pixels at the edge of the image (the most common technique).   !!!!!!!!!
  3. More sophisticated techniques.

  Once the dummy data is added, the blocks go through the rest of the JPEG encoding process.

  Finally, the additional pixels are removed.
  */

 uint32_t numRows = yCbCr.size();
 uint32_t numCols = yCbCr[0].size();

  // Pad same pixels through the padded columns
  for (uint32_t rowIdx = 0; rowIdx < originalHeight; ++rowIdx)
  {

    YCbCr_Val valueToCopy = yCbCr[rowIdx][originalWidth - 1];

    for (uint32_t colIdx = originalWidth; colIdx < numCols; ++colIdx)
    {
      yCbCr[rowIdx][colIdx] = valueToCopy;
    }
  }

  // Pad same pixels through the padded rows
  for (uint32_t colIdx = 0; colIdx < originalWidth; ++colIdx)
  {

    YCbCr_Val valueToCopy = yCbCr[originalHeight - 1][colIdx];

    for (uint32_t rowIdx = originalHeight; rowIdx < numRows; ++rowIdx)
    {
      yCbCr[rowIdx][colIdx] = valueToCopy;
    }
  }

  // for the corner pixels
  YCbCr_Val valueToCopy = yCbCr[originalHeight - 1][originalWidth - 1];

  for (uint32_t rowIdx = originalHeight; rowIdx < yCbCr.size(); ++rowIdx)
  {
    for (uint32_t colIdx = originalWidth; colIdx < yCbCr[0].size(); ++colIdx)
    {
      yCbCr[rowIdx][colIdx] = valueToCopy;
    }
  }
}