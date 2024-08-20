#include "../src/JpegCompression.hpp"
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <array>
#include <string>
#include <math.h>

#define EPSILON 1e-2

const std::string DCT_TEST_DIR = "test/dctTesting/";

const std::string BASE_MATRIX_FOLDER = DCT_TEST_DIR + "dct_test_imgs/";
const std::string BASE_CORRECT_FOLDER = DCT_TEST_DIR + "correct_image_values/";

void run_dct_test(int test_idx)
{
  std::cout << "Testing Matrix " << test_idx << std::endl;

  // reading in Matrix we are testing
  std::ifstream original_matrix_file(BASE_MATRIX_FOLDER + "test_" + std::to_string(test_idx) + ".txt");

  YCbCrImgMatrix matrix_to_test;

  matrix_to_test.resize(8);

  for (int i = 0; i < 8; ++i)
  {
    matrix_to_test[i].resize(8);
  }

  double read_Y, read_Cb, read_Cr;

  for (uint8_t rowIdx = 0; rowIdx < 8; ++rowIdx)
  {
    for (uint8_t colIdx = 0; colIdx < 8; ++colIdx)
    {
      original_matrix_file >> read_Y >> read_Cb >> read_Cr;

      matrix_to_test[rowIdx][colIdx].y = read_Y;
      matrix_to_test[rowIdx][colIdx].cb = read_Cb;
      matrix_to_test[rowIdx][colIdx].cr = read_Cr;
    }
  }

  // performs inplace DCT operation
  performDCT(matrix_to_test);

  // comparing to true correct matrix output
  std::ifstream correct_matrix_file(BASE_CORRECT_FOLDER + "test_" + std::to_string(test_idx) + "_correct.txt");

  for (uint8_t rowIdx = 0; rowIdx < 8; ++rowIdx)
  {
    for (uint8_t colIdx = 0; colIdx < 8; ++colIdx)
    {
      correct_matrix_file >> read_Y >> read_Cb >> read_Cr;

      assert(fabs(matrix_to_test[rowIdx][colIdx].y - read_Y) < EPSILON);
      assert(fabs(matrix_to_test[rowIdx][colIdx].cb - read_Cb) < EPSILON);
      assert(fabs(matrix_to_test[rowIdx][colIdx].cr - read_Cr) < EPSILON);
    }
  }

  std::cout << "Matrix Correct!" << std::endl;
  std::cout << "@@@ Passed" << std::endl << std::endl;
}

int main(
  void)
{

  for (int test_idx = 0; test_idx < 10; ++test_idx)
  {
    run_dct_test(test_idx);
  }

  std::cout << "@@@ ALL TEST PASSED :)" << std::endl << std::endl;
}