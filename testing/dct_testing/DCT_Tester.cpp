#include "../../Discrete_Cosine_Transformation.hpp"
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <array>
#include <string>
#include <math.h>

#define EPSILON 1e-2

const std::string BASE_MATRIX_FOLDER = "testing/dct_testing/dct_test_imgs/";
const std::string BASE_CORRECT_FOLDER = "testing/dct_testing/correct_image_values/";

void run_dct_test(int test_idx) {

    std::cout << "Testing Matrix " << test_idx << std::endl;

    // reading in Matrix we are testing
    std::ifstream original_matrix_file(BASE_MATRIX_FOLDER + "test_" + std::to_string(test_idx) + ".txt");

    YCbCr_Img_Matrix matrix_to_test;

    matrix_to_test.resize(8);

    for (int i = 0; i < 8; ++i) {
        matrix_to_test[i].resize(8);
    }


    double read_Y, read_Cb, read_Cr;

    for (uint8_t row_idx = 0; row_idx < 8; ++row_idx) {
        for (uint8_t col_idx = 0; col_idx < 8; ++col_idx) {
            original_matrix_file >> read_Y >> read_Cb >> read_Cr;

            matrix_to_test[row_idx][col_idx].y = read_Y;
            matrix_to_test[row_idx][col_idx].cb = read_Cb;
            matrix_to_test[row_idx][col_idx].cr = read_Cr;
        }
    }

    // performs inplace DCT operation
    perform_DCT_operation(matrix_to_test);
    
    // comparing to true correct matrix output
    std::ifstream correct_matrix_file(BASE_CORRECT_FOLDER + "test_" + std::to_string(test_idx) + "_correct.txt");

    for (uint8_t row_idx = 0; row_idx < 8; ++row_idx) {
        for (uint8_t col_idx = 0; col_idx < 8; ++col_idx) {

            correct_matrix_file >> read_Y >> read_Cb >> read_Cr;

            assert(fabs(matrix_to_test[row_idx][col_idx].y - read_Y) < EPSILON);
            assert(fabs(matrix_to_test[row_idx][col_idx].cb - read_Cb) < EPSILON);
            assert(fabs(matrix_to_test[row_idx][col_idx].cr - read_Cr) < EPSILON);
        }
    }
    
    std::cout << "Matrix Correct!" << std::endl;
    std::cout << "@@@ Passed" << std::endl << std::endl;

    return;
}

int main(int argc, char ** argv) {

    for (int test_idx = 0; test_idx < 10; ++test_idx) {
        run_dct_test(test_idx);
    }
    
    std::cout << "@@@ ALL TEST PASSED :)" << std::endl << std::endl;
}