import numpy as np

BASE_TESTING_FOLDER = "testing/dct_testing/"


def print_to_file(input_bitmap_matrix, filename):
    
    string_to_file = ""
    
    for row in range(8):
        
        for col in range(8):
            
            string_to_file += str(input_bitmap_matrix[row][col]) + " "
        
        string_to_file += "\n"
    
    correct_value_file = open(filename, "w")
    correct_value_file.write(string_to_file)
    correct_value_file.close()
    
    return 


def main():
    
    dct_matrix = np.zeros((8,8))

    dct_matrix[0,:] = np.sqrt(1/8)

    for row in range(1, 8):
        for col in range(8):
            dct_matrix[row][col] = 0.5 * np.cos((2*col + 1) * row * np.pi / 16)
    
    for test_idx in range(10):
        
        original_matrix = np.random.rand(8, 8) * 256
    
        print_to_file(original_matrix, BASE_TESTING_FOLDER + "dct_test_imgs/test_" + str(test_idx) + ".txt")
        
        original_matrix = original_matrix - 128
        
        dct_coefficients = np.matmul(np.matmul(dct_matrix, original_matrix), dct_matrix.T)
        
        print_to_file(dct_coefficients, BASE_TESTING_FOLDER + "correct_image_values/test_" + str(test_idx) + "_correct.txt")
        
    return

main()