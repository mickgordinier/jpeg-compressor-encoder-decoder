#include "src/JpegCompression.hpp"

int main(int argc, char* argv[]) 
{
    /* Performing JPEG Compression and Encoding */
    /* Arguments: ./a.out <BMP_IMAGE>.bmp */
    if (argc != 2) {
        std::cout << "Need to input BMP file" << std::endl;
        exit(1);
    }

    /* TODO: Check type of file and confirm it ends in .bmp */
    
    // Step 1: Decode uncompressed Bitmap file and obtain RGB matrix to encode
    std::string filename = argv[1];
    BitmapDecoder bitmap(filename);
    bitmap.createOutputFile("testOut.bmp", bitmap.rgbImgMatrix);

    // // Step 2: Convert RGB matrix to yCbCr Matrix to extract luminance channel from chroma/color channels
    // // Padding image size to multiple of 8 to have correct MCU size needed for DCT
    // // Padded pixels will be ignored afterwards
    // YCbCrImgMatrix convertedImage = convertRgbToPaddedYCbCr(bitmap.rgbImgMatrix);

    // // // Step 3: Downsampling Chroma Components TBD


    // // Step 4: Discrete Cosine Transform (DCT)
    // // Transforming 8x8 image blocks from spacial domain --> "frequency domain"
    // // Will be applied to each channel
    // // Performing an in-place DCT transformation on an 8x8 pixel block within the YCbCr Image
    // // DCT Transformation = (DCT Matrix) * (Original Image) * (DCT Matrix Transformed)
    // performDCT(convertedImage);

    // // Step 5: Quantization
    // // Removing information from the DCT image
    // // Image Quality - # btw. 0 - 100 to specify how much information you would like to keep
    // // The higher the quality, the higher the number of information that will stay on the image
    // quantization(convertedImage, 50);

    // compressedImage = 

    // // Step 6: Entropy Encoding
    
    return 0;
}