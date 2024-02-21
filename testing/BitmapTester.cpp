#include "../BitmapDecoder.hpp"
#include <iostream>
#include <cassert>

// running comparisons on all features of BitmapDecoder
int runTest(BitmapDecoder & bit_test, std::vector< std::vector< std::array<uint8_t, 3>>> & correct_matrix, 
            uint32_t correct_height, uint32_t correct_width) {
        
    std::cout << "Calculated Height: " << bit_test.height << ",  Correct Height: " << correct_height << std::endl;
    assert(bit_test.height == correct_height);

    std::cout << "Calculated Width: " << bit_test.width << ",  Correct Width: " << correct_width << std::endl;
    assert(bit_test.width == correct_width);

    std::cout << "Comparing Image Matrix" << std::endl;
    for (uint32_t row_idx = 0; row_idx < correct_height; ++row_idx) {
        for (uint32_t col_idx = 0; col_idx < correct_width; ++col_idx) {
            for (uint8_t color_idx = 0; color_idx < 3; ++color_idx) {
                assert(bit_test.rgb_img_matrix[row_idx][col_idx][color_idx] == correct_matrix[row_idx][col_idx][color_idx]);
            }
        }
    }
    std::cout << "Image Matrix Correct!" << std::endl;
    std::cout << "@@@ Passed" << std::endl;
}


int main(int argc, char ** argv) {
    std::cout << "Hello" << std::endl;
    std::string filename = "bitmap_test_imgs/test_1.bmp";
    BitmapDecoder bit_test_1(filename);
    std::cout << "Bitmap Image Height: " << bit_test_1.height << std::endl;
    std::cout << "Bitmap Image Width: " << bit_test_1.width << std::endl << std::endl;

    std::cout << "RGB Matrix" << std::endl;
    bit_test_1.printRgbMatrix();
}