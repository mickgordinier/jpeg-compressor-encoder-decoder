#ifndef BITMAPDECODER_H
#define BITMAPDECODER_H

#include <iostream>
#include <array>
#include <vector>

class BitmapDecoder {
    private:
        void createRgbMatrix(std::ifstream& bmp_file);
        uint32_t getWord(std::ifstream& bmp_file);
        void skipBytes(std::ifstream& bmp_file, std::uint8_t bytes_to_skip);
    
    public:
        BitmapDecoder(std::string filename);
        void printRgbMatrix();
        
        std::vector< std::vector< std::array<uint8_t, 3>>> rgb_img_matrix;
        uint32_t height;
        uint32_t width;
};

#endif //BITMAPDECODER_H