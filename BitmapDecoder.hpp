#ifndef BITMAPDECODER_H
#define BITMAPDECODER_H

#include "CommonTypes.hpp"
#include <iostream>
#include <array>
#include <vector>

class BitmapDecoder {
    private:
        void createRgbMatrix(std::ifstream& bmp_file);
        uint32_t getWord(std::ifstream& bmp_file);
        void skipBytes(std::ifstream& bmp_file, uint8_t bytes_to_skip);
    
    public:
        BitmapDecoder(std::string filename);
        void printRgbMatrix();
        
        RGB_Img_Matrix rgb_img_matrix;
        uint32_t height;
        uint32_t width;
};

#endif //BITMAPDECODER_H