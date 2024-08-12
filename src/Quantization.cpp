#include "JpegCompression.hpp"

/*
 * Source: https://www.w3.org/Graphics/JPEG/itu-t81.pdf
 * These are based on psychovisual thresholding and
 * are derived empirically using luminance and chrominance and 2:1 horizontal subsampling. These tables are provided as
 * examples only and are not necessarily suitable for any particular application. These quantization values have been used
 * with good results on 8-bit per sample luminance and chrominance images of the format illustrated in Figure 13. Note that
 * these quantization values are appropriate for the DCT normalization defined in A.3.3.
 * If these quantization values are divided by 2, the resulting reconstructed image is usually nearly indistinguishable from the
 * source image.
 */

// https://www.cl.cam.ac.uk/teaching/1011/R08/jpeg/acs10-jpeg.pdf
// https://www.sciencedirect.com/topics/engineering/quantization-table
// https://www.sciencedirect.com/science/article/pii/S1742287608000285

// Removing high frequencies present in original image --> Losing high detail for smaller file size
void quantization(
  YCbCrImgMatrix &dctImage,
  uint8_t qualityFactor)
{
  double scalingFactor = 0;
  if (qualityFactor < 50) {
    scalingFactor = 5000 / qualityFactor;
  } else {
    scalingFactor = 200 - (2 * qualityFactor);
  }
  std::cout << "Scaling Factor: " << scalingFactor << std::endl;

  for (std::uint32_t rowIdx = 0; rowIdx < dctImage.size(); ++rowIdx)
  {
    for (std::uint32_t colIdx = 0; colIdx < dctImage[0].size(); ++colIdx)
    {
      double qLumanice = ((scalingFactor * LUMINANCE_QUANTIZATION_MATRIX[rowIdx % 8][colIdx % 8]) + 50) / 100;
      double qChroma = ((scalingFactor * CHROMA_QUANTIZATION_MATRIX[rowIdx % 8][colIdx % 8]) + 50) / 100;

      dctImage[rowIdx][colIdx].y = std::round(dctImage[rowIdx][colIdx].y / qLumanice);
      dctImage[rowIdx][colIdx].cb = std::round(dctImage[rowIdx][colIdx].cb / qChroma);
      dctImage[rowIdx][colIdx].cr = std::round(dctImage[rowIdx][colIdx].cr / qChroma);
    }
  }
}


void decompressImage(
  YCbCrImgMatrix &dctImage,
  uint8_t qualityFactor)
{
  double scalingFactor = 0;
  if (qualityFactor < 50) {
    scalingFactor = 5000 / qualityFactor;
  } else {
    scalingFactor = 200 - (2 * qualityFactor);
  }

  for (std::uint32_t rowIdx = 0; rowIdx < dctImage.size(); ++rowIdx)
  {
    for (std::uint32_t colIdx = 0; colIdx < dctImage[0].size(); ++colIdx)
    {
      double qLumanice = ((scalingFactor * LUMINANCE_QUANTIZATION_MATRIX[rowIdx % 8][colIdx % 8]) + 50) / 100;
      double qChroma = ((scalingFactor * CHROMA_QUANTIZATION_MATRIX[rowIdx % 8][colIdx % 8]) + 50) / 100;

      dctImage[rowIdx][colIdx].y = dctImage[rowIdx][colIdx].y * qLumanice;
      dctImage[rowIdx][colIdx].cb = dctImage[rowIdx][colIdx].cb * qChroma;
      dctImage[rowIdx][colIdx].cr = dctImage[rowIdx][colIdx].cr * qChroma;
    }
  }
}