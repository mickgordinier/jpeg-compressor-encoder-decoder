#include "BitmapDecoder.hpp"

int main(int argc, char* argv[]) 
{
    std::string filename = "test_img/test_0.bmp";
    BitmapDecoder bitmap(filename);

    std::cout << "Bitmap Image Height: " << bitmap.height << std::endl;
    std::cout << "Bitmap Image Width: " << bitmap.width << std::endl;
    std::cout << "Bitmap Image Size: " << bitmap.image_size << std::endl << std::endl;

    bitmap.printRgbMatrix();
}