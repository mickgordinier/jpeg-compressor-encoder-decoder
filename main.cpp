#include "CommonTypes.hpp"
#include "BitmapDecoder.hpp"
#include "Discrete_Cosine_Transformation.hpp"
#include "YCbCr_Transformation.hpp"
#include "Quantization.hpp"

// Prints similar to format as numpy matrix on python
/*
void print_image_matrix(RGB_Image_Matrix & image_matrix, std::ostream& stream) {

    for (uint8_t color_layer = 0; color_layer < 3; ++color_layer) {
        stream << "[";
        for (uint32_t row_idx = 0; row_idx < image_matrix.size(); ++row_idx) {
            if (row_idx != 0) {
                stream << " ";
            }

            stream << "[";

            for (uint32_t col_idx = 0; col_idx < image_matrix[0].size(); ++col_idx) {
                uint8_t value = unsigned(image_matrix[row_idx][col_idx][color_layer]);

                uint8_t numspaces = (value / 100 <= 0) + (value / 10 <= 0);
                for (uint8_t i = 0; i < numspaces; ++i) {
                    stream << " ";
                }

                stream << unsigned(value);

                if (col_idx < image_matrix[0].size()-1) {
                    stream << ", ";
                }
            }
            stream << "]";

            if (row_idx < image_matrix.size()-1) {
                stream << "," << std::endl;
            }
        }

        stream << "]" << std::endl << std::endl;
    }
    stream << std::endl;
}
*/


int main(int argc, char* argv[]) 
{
    if (argc != 2) {
        std::cout << "Need to input BMP file" << std::endl;
        exit(1);
    }
    
    // Step 1: Decode uncompressed Bitmap file and obtain RGB matrix to encode
    std::string filename = argv[1];
    BitmapDecoder bitmap(filename);

    // Step 2: Convert RGB matrix to yCbCr Matrix to extract luminance channel from chroma/color channels
    // Padding image size to multiple of 8 to have correct MCU size needed for DCT
    // Padded pixels will be ignored afterwards
    YCbCr_Img_Matrix converted_image = convert_RGB_to_padded_YCbCr(bitmap.rgb_img_matrix);

    // Step 3: Downsampling Chroma Components TBD


    // Step 4: Discrete Cosine Transform (DCT)
    // Transforming 8x8 image blocks from spacial domain --> "frequency domain"
    // Will be applied to each channel
    perform_DCT_operation(converted_image);

    // Step 5: Quantization
    // Removing information from the DCT image
    // Image Quality - # btw. 0 - 100 to specify how much information you would like to keep
    // The higher the quality, the higher the number of information that will stay on the image
    quantization(converted_image, 50);

    // Step 6: Entropy Encoding
    
    return 0;
}