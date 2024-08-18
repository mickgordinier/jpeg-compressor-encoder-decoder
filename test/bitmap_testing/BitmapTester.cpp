#include "../../src/JpegCompression.hpp"
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <array>
#include <filesystem>
#include <string>

/*
IMAGE DESCRIPTIONS IN test_img_description.txt!
*/

const std::string BITMAP_TEST_DIR = "test/bitmap_testing/";

const std::string BASE_IMAGE_FOLDER = BITMAP_TEST_DIR + "bitmap_test_imgs/";
const std::string BASE_CORRECT_FOLDER = BITMAP_TEST_DIR + "correct_image_values/";

// running comparisons on all features of BitmapDecoder
int runBitmapDecoderTest(std::string bmpFileName) {
    
    const std::string bitmapImagePath = BASE_IMAGE_FOLDER + bmpFileName + ".bmp";
    const std::string correctValuesPath = BASE_CORRECT_FOLDER + bmpFileName + "_correct.txt";

    std::cout << "Testing Image " << bmpFileName << std::endl;
    std::cout << "Bitmap Image Path: " << bitmapImagePath << std::endl;
    std::cout << "Correct Values Path: " << correctValuesPath << std::endl;

    BitmapDecoder decodedBitmap(bitmapImagePath);

    std::ifstream correctFileStream(correctValuesPath);

    if (!correctFileStream) {
      std::cout << "ERROR: File stream (" << correctValuesPath << ") not able to be opened" << std::endl;
      exit(-1);
    }

    uint32_t correct_height, correct_width;
    correctFileStream >> correct_height >> correct_width;

    std::cout << "Calculated Height: " << decodedBitmap.height << ",  Correct Height: " << correct_height << std::endl;
    assert(decodedBitmap.height == correct_height);

    std::cout << "Calculated Width: " << decodedBitmap.width << ",  Correct Width: " << correct_width << std::endl;
    assert(decodedBitmap.width == correct_width);

    std::cout << "Comparing Image Matrix" << std::endl;

    uint32_t color_val_r, color_val_g, color_val_b;

    for (uint32_t rowIdx = 0; rowIdx < correct_height; ++rowIdx) {
        for (uint32_t colIdx = 0; colIdx < correct_width; ++colIdx) {
            correctFileStream >> color_val_r >> color_val_g >> color_val_b;

            assert(decodedBitmap.rgbImgMatrix[rowIdx][colIdx].r == color_val_r);
            assert(decodedBitmap.rgbImgMatrix[rowIdx][colIdx].g == color_val_g);
            assert(decodedBitmap.rgbImgMatrix[rowIdx][colIdx].b == color_val_b);
        }
    }
    std::cout << "Image Matrix Correct!" << std::endl;
    std::cout << "@@@ Passed " << bmpFileName << std::endl << std::endl;
    return 0;
}


int main(
  void) 
{
  std::cout << "Testing Bitmap Creation" << std::endl << std::endl;

  std::string bmpFileName;

  for (int test_idx = 1; test_idx <= 8; ++test_idx) {
    bmpFileName = "test_" + std::to_string(test_idx);
    runBitmapDecoderTest(bmpFileName);
  }
  
  std::cout << "@@@ ALL TEST PASSED :)" << std::endl << std::endl;
}