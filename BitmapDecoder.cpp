#include "BitmapDecoder.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <cstdint>

typedef std::array<uint8_t, 3> RGB_Val;                  // Can be uint_8 as values range from 0-255
typedef std::vector<std::vector<RGB_Val>> RGB_Img_Matrix;

BitmapDecoder::BitmapDecoder(std::string filename) {
    
    // opening up bmp img bytes to read
    // setting i/o stream base mode to binary
    std::ifstream bmp_file(filename, std::ios_base::binary);

    // READING IN BITMAP HEADER AND INFOHEADER

    // ignore first 2 bytes (header signature == "BM")
    // ignore next 4 bytes (file size in bytes)
    // ignore next 4 byes (unused)
    // ignore next 4 bytes (offset from beginning of file to beginning of bitmap data)
    // ignore next 4 bytes (size of InfoHeader = 40 bytes)
    BitmapDecoder::skipBytes(bmp_file, 18);

    // getting horizontal width of Bitmap in pixels
    width = BitmapDecoder::getWord(bmp_file);

    // getting horizontal height of Bitmap in pixels
    height = BitmapDecoder::getWord(bmp_file);

    // reserving space for RGB Matrix
    rgb_img_matrix.resize(height);
    for (uint32_t row_idx = 0; row_idx < height; ++row_idx) {
        rgb_img_matrix[row_idx].resize(width);
    }

    // ignore next 2 bytes (# of planes = 1)
    // ignore next 2 bytes (Bits per pixel = 24 --> 24-bit RGB --> one-byte each)
    // ignore next 4 bytes (Compression type = 0 --> no compression)
    // ignore next 4 bytes (Image size in bytes)
    // ignore next 4 bytes (horizontal resolution in pixel/meter)
    // ignore next 4 bytes (vertical resolution in pixel/meter)
    // ignore next 8 bytes (color used and important colors)
    BitmapDecoder::skipBytes(bmp_file, 28);

    // creating 3D vector of size height x width x 3 layers for RGB
    BitmapDecoder::createRgbMatrix(bmp_file);
}


// return nexts 4 bytes as little endian uint32_t
std::uint32_t BitmapDecoder::getWord(std::ifstream& bmp_file) {
    std::uint32_t word = (bmp_file.get()) +
                        (bmp_file.get() << 8) +
                        (bmp_file.get() << 16) +
                        (bmp_file.get() << 24);
    return word;
}


void BitmapDecoder::skipBytes(std::ifstream& bmp_file, std::uint8_t bytes_to_skip) {
    for (uint8_t i = 0; i < bytes_to_skip; ++i) {
        bmp_file.get();
    }
}


void BitmapDecoder::createRgbMatrix(std::ifstream& bmp_file) {
    
    std::uint8_t file_byte;

    //https://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm
    // Each scan line is zero padded to the nearest 4-byte boundary. If the image has a width that is not divisible by four, say, 21 bytes, 
    // there would be 3 bytes of padding at the end of every scan line.
    std::uint8_t num_padding_bytes = 0;
    if ((width*3)%4 != 0) {
        num_padding_bytes = 4 - (width*3)%4;
    }

    // image read goes bottom-up
    for (std::uint32_t row_idx = height; row_idx > 0; --row_idx) {

        // image read goes from left to right, 3 bytes per pixel
        for (std::uint32_t col_idx = 0; col_idx < width; ++col_idx) {

            // 0 = red, 1 = green, 2 = blue
            for (std::uint8_t color_layer_idx = 3; color_layer_idx > 0; --color_layer_idx) {
                file_byte = bmp_file.get();

                rgb_img_matrix[row_idx-1][col_idx][color_layer_idx-1] = file_byte;
            }
        }

        // accounting for padding bytes in file
        skipBytes(bmp_file, num_padding_bytes);
    }
}


// Prints similar to format as numpy matrix on python
void BitmapDecoder::printRgbMatrix() {

    for (uint8_t color_layer = 0; color_layer < 3; ++color_layer) {
        std::cout << "[";
        for (uint32_t row_idx = 0; row_idx < rgb_img_matrix.size(); ++row_idx) {
            if (row_idx != 0) {
                std::cout << " ";
            }

            std::cout << "[";

            for (uint32_t col_idx = 0; col_idx < rgb_img_matrix[0].size(); ++col_idx) {
                uint8_t value = unsigned(rgb_img_matrix[row_idx][col_idx][color_layer]);

                uint8_t numspaces = (value / 100 <= 0) + (value / 10 <= 0);
                for (uint8_t i = 0; i < numspaces; ++i) {
                    std::cout << " ";
                }

                std::cout << unsigned(value);

                if (col_idx < rgb_img_matrix[0].size()-1) {
                    std::cout << ", ";
                }
            }
            std::cout << "]";

            if (row_idx < rgb_img_matrix.size()-1) {
                std::cout << "," << std::endl;
            }
        }

        std::cout << "]" << std::endl << std::endl;
    }
    std::cout << std::endl;
}