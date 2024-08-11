#include "../../BitmapDecoder.hpp"
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

const std::string BASE_IMAGE_FOLDER = "testing/bitmap_testing/bitmap_test_imgs/";
const std::string BASE_CORRECT_FOLDER = "testing/bitmap_testing/correct_image_values/";

// running comparisons on all features of BitmapDecoder
void runBitmapDecoderTest(std::string bmp_file_name) {
        
    std::cout << "Testing Image " << bmp_file_name << std::endl;

    BitmapDecoder bit_test(BASE_IMAGE_FOLDER + bmp_file_name + ".bmp");

    std::ifstream correct_value_file(BASE_CORRECT_FOLDER + bmp_file_name + "_correct.txt");

    uint32_t correct_height, correct_width;
    correct_value_file >> correct_height >> correct_width;

    std::cout << "Calculated Height: " << bit_test.height << ",  Correct Height: " << correct_height << std::endl;
    assert(bit_test.height == correct_height);

    std::cout << "Calculated Width: " << bit_test.width << ",  Correct Width: " << correct_width << std::endl;
    assert(bit_test.width == correct_width);

    std::cout << "Comparing Image Matrix" << std::endl;

    uint32_t color_val_r, color_val_g, color_val_b;

    for (uint32_t rowIdx = 0; rowIdx < correct_height; ++rowIdx) {
        for (uint32_t colIdx = 0; colIdx < correct_width; ++colIdx) {
            correct_value_file >> color_val_r >> color_val_g >> color_val_b;

            assert(bit_test.RgbImgMatrix[rowIdx][colIdx].r == color_val_r);
            assert(bit_test.RgbImgMatrix[rowIdx][colIdx].g == color_val_g);
            assert(bit_test.RgbImgMatrix[rowIdx][colIdx].b == color_val_b);
        }
    }
    std::cout << "Image Matrix Correct!" << std::endl;
    std::cout << "@@@ Passed" << std::endl << std::endl;
}


int main(int argc, char ** argv) {
    std::cout << "Testing Bitmap Creation" << std::endl << std::endl;

    std::string bmp_file_name;

    for (int test_idx = 1; test_idx <= 8; ++test_idx) {
        bmp_file_name = "test_" + std::to_string(test_idx);
        runBitmapDecoderTest(bmp_file_name);
    }
    
    std::cout << "@@@ ALL TEST PASSED :)" << std::endl << std::endl;
}