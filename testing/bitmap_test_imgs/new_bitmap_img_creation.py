"""
Name: Mick Gordinier
Date: 05/20/2024

Generates new BITMAP Image file to be used for testing purposes
Can also now create additional folder that hold the correct values when decoding the BITMAP
"""

import numpy as np

# Given numpy matrix, place needed info to go on the test_x_correct.txt file
# NOTE: Each RGB For Pixel will go as "RED GREEN BLUE"
"""
CORRECT VALUE FILE FORMAT

# ROWS
# COLS
(Pixel (0,0) RGB) (Pixel (0,1) RGB) ... (Pixel (0,# COLS) RGB)
      .
      .
      .
(Pixel (# ROWS,0) RGB) (Pixel (# ROWS,1) RGB) ... (Pixel (# ROWS,# COLS) RGB)
"""
def print_correct_value_file_format(input_bitmap_matrix, filename):
    
    string_to_file = ""
    
    string_to_file += str(input_bitmap_matrix.shape[0]) + "\n"
    string_to_file += str(input_bitmap_matrix.shape[1]) + "\n"
    
    for row in range(input_bitmap_matrix.shape[0]):
        
        for col in range(input_bitmap_matrix.shape[1]):
            
            for rgb_val in range(3):
                if (input_bitmap_matrix[row][col][rgb_val] < 10):
                    num_spaces = 2
                elif (input_bitmap_matrix[row][col][rgb_val] < 100):
                    num_spaces = 1
                else:
                    num_spaces = 0
                    
                string_to_file += (" " * num_spaces) + str(input_bitmap_matrix[row][col][rgb_val]) + " "
            
            string_to_file += " "
        
        string_to_file += "\n"
    
    correct_value_file = open(filename, "w")
    correct_value_file.write(string_to_file)
    correct_value_file.close()
    
    return 

def main():
    
    filename = "test_3"
    
    input_bitmap_matrix = [
[[255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255]],
[[255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255]],
[[255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255]],
[[255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255]],
[[255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255]],
[[255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255]],
[[255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255]],
[[255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255]],
[[255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255], [255, 255, 255]]
] 
    
    input_bitmap_matrix = np.array(input_bitmap_matrix)
    
    BASE_CORRECT_VALUES_FOLDER = "testing/correct_image_values/"
    
    print_correct_value_file_format(input_bitmap_matrix, BASE_CORRECT_VALUES_FOLDER + filename + "_correct.txt")
    
    return

main()