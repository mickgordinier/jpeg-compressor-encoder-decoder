#include "BitmapDecoder.hpp"
#include <cmath>


// Prints similar to format as numpy matrix on python
void print_image_matrix(std::vector< std::vector< std::array<uint8_t, 3>>> & image_matrix, std::ostream& stream) {

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


// creates the 8x8 dct
void create_discrete_cosine_transform_matrix(std::vector<std::vector<double>> & dct) {
    
    // entire first row is sqrt(1/N)
    // using double for more precise percision
    double dct_val = std::sqrt(1/8);
    for (std::uint8_t col_idx = 0; col_idx < 8; ++col_idx) {
        dct[0][col_idx] = dct_val;
    }

    // all values multiplied by sqrt(2/8) == sqrt(1/4) == 1/2
    for (std::uint8_t row_idx = 1; row_idx < 8; ++row_idx) {
        for (std::uint8_t col_idx = 0; col_idx < 8; ++col_idx) {
            dct[row_idx][col_idx] = 0.5 * cos(((2*col_idx + 1) * row_idx * M_PI) / 16);
        }
    }
}


void convert_RGB_to_YCbCr(std::vector< std::vector< std::array<uint8_t, 3>>> & rgb, std::vector< std::vector< std::array<uint8_t, 3>>> & yCbCr) {
    for (uint32_t row_idx = 0; row_idx < rgb.size(); ++row_idx) {
        for (uint32_t col_idx = 0; col_idx < rgb[0].size(); ++col_idx) {
            
            // luminance channel
            yCbCr[row_idx][col_idx][0] = (rgb[row_idx][col_idx][0] * 0.299) + (rgb[row_idx][col_idx][1] * 0.587) + (rgb[row_idx][col_idx][2] * 0.114);
            
            // chroma blue channel
            yCbCr[row_idx][col_idx][1] = (rgb[row_idx][col_idx][0] * -0.1687) + (rgb[row_idx][col_idx][1] * -0.331) + (rgb[row_idx][col_idx][2] * 0.5) + 128;
            
            // chroma red channel
            yCbCr[row_idx][col_idx][2] = (rgb[row_idx][col_idx][0] * 0.5) + (rgb[row_idx][col_idx][1] * -0.4187) + (rgb[row_idx][col_idx][2] * -0.0813) + 128;
        }
    }
}


void pad_YCbCr(std::vector< std::vector< std::array<uint8_t, 3>>> & yCbCr, uint32_t original_height, uint32_t original_width) {
    
    // Pad same pixels through the padded columns
    for (uint32_t col_idx = original_width; col_idx < yCbCr[0].size(); ++col_idx) {
        for (uint32_t row_idx = 0; row_idx < original_height; ++row_idx) {
            yCbCr[row_idx][col_idx][0] = yCbCr[row_idx][original_width-1][0];
            yCbCr[row_idx][col_idx][1] = yCbCr[row_idx][original_width-1][1];
            yCbCr[row_idx][col_idx][2] = yCbCr[row_idx][original_width-1][2];
        }
    }

    // Pad same pixels through the padded rows
    for (uint32_t row_idx = original_height; row_idx < yCbCr.size(); ++row_idx) {
        for (uint32_t col_idx = 0; col_idx < original_width; ++col_idx) {
            yCbCr[row_idx][col_idx][0] = yCbCr[original_height-1][col_idx][0];
            yCbCr[row_idx][col_idx][1] = yCbCr[original_height-1][col_idx][1];
            yCbCr[row_idx][col_idx][2] = yCbCr[original_height-1][col_idx][2];
        }
    }

    // for the corner pixels
    for (uint32_t row_idx = original_height; row_idx < yCbCr.size(); ++row_idx) {
        for (uint32_t col_idx = original_width; col_idx < yCbCr[0].size(); ++col_idx) {
            yCbCr[row_idx][col_idx][0] = yCbCr[original_height-1][original_width-1][0];
            yCbCr[row_idx][col_idx][1] = yCbCr[original_height-1][original_width-1][1];
            yCbCr[row_idx][col_idx][2] = yCbCr[original_height-1][original_width-1][2];
        }
    }
}


int main(int argc, char* argv[]) 
{
    if (argc != 2) {
        std::cout << "Need to input BMP file" << std::endl;
        exit(1);
    }
    
    // Step 1: Decode uncompressed Bitmap file and obtain RGB matrix to encode
    std::string filename = argv[1];
    BitmapDecoder bitmap(filename);

    std::cout << "Bitmap Image Height: " << bitmap.height << std::endl;
    std::cout << "Bitmap Image Width: " << bitmap.width << std::endl << std::endl;

    std::cout << "RGB Matrix" << std::endl;
    print_image_matrix(bitmap.rgb_img_matrix, std::cout);

    // Step 2: Convert RGB matrix to yCbCr Matrix to extract luminance channel from chroma/color channels
    std::vector< std::vector< std::array<uint8_t, 3>>> yCbCr_img_matrix;

    // reserving space for YCbCr Matrix
    // NEED HEIGHT AND WIDTH TO BE MULTIPLE OF 8 TO PROPERLY PERFORM DCT
    // WILL IGNORE PADDED PIXELS AFTERWARDS
    uint32_t yCbCr_height = bitmap.height;
    uint32_t yCbCr_width = bitmap.width;

    if (yCbCr_height % 8 != 0) {
        yCbCr_height += 8 - yCbCr_height%8;
    }

    if (yCbCr_width % 8 != 0) {
        yCbCr_width += 8 - yCbCr_width%8;
    }

    yCbCr_img_matrix.resize(yCbCr_height);
    for (uint32_t row_idx = 0; row_idx < yCbCr_height; ++row_idx) {
        yCbCr_img_matrix[row_idx].resize(yCbCr_width);
    }

    convert_RGB_to_YCbCr(bitmap.rgb_img_matrix, yCbCr_img_matrix);
    std::cout << "Unpadded YCbCr Matrix" << std::endl;
    print_image_matrix(yCbCr_img_matrix, std::cout);

    // adding additional padded pixels to edges to get to row,col to be multiple of 8 needed for DCT
    // Will need more functionality when downsampling
    pad_YCbCr(yCbCr_img_matrix, bitmap.height, bitmap.width);
    std::cout << "Padded YCbCr Matrix" << std::endl;
    print_image_matrix(yCbCr_img_matrix, std::cout);

    // Step 3: Downsampling Chroma Components TBD


    // Step 4: DCT
}