import numpy as np
from scipy.fftpack import dct

BASE_TESTING_FOLDER = "testing/dct_testing/"

def print_to_file(input_bitmap_matrix, filename):
    
    string_to_file = ""
    
    for row in range(8):
        
        for col in range(8):
            
            for model_idx in range(3):
            
                string_to_file += str(np.round(input_bitmap_matrix[row][col][model_idx], 4)) + " "
        
        string_to_file += "\n"
    
    correct_value_file = open(filename, "w")
    correct_value_file.write(string_to_file)
    correct_value_file.close()
    
    return 


def main():
    
    dctMatrix = np.zeros((8,8))

    dctMatrix[0,:] = np.sqrt(1/8)

    for row in range(1, 8):
        for col in range(8):
            dctMatrix[row][col] = 0.5 * np.cos((2*col + 1) * row * np.pi / 16)
    
    for test_idx in range(1):
        
        original_matrix = np.random.rand(8, 8, 3) * 256
        dct_coefficients = np.zeros((8,8,3))
    
        print_to_file(original_matrix, BASE_TESTING_FOLDER + "dct_test_imgs/test_" + str(test_idx) + ".txt")
        
        original_matrix = original_matrix - 128
        
        for model_idx in range(3):
            dct_coefficients[:, :, model_idx] = dct(dct(original_matrix[:,:,model_idx].T, norm='ortho').T, norm='ortho')
            # dct_coefficients[:, :, model_idx] = np.matmul(np.matmul(dctMatrix, original_matrix[:, :, model_idx]), dctMatrix.T)
        
        print_to_file(dct_coefficients, BASE_TESTING_FOLDER + "correct_image_values/test_" + str(test_idx) + "_correct.txt")
        
    
    """
    for i in range(8):
        hi = ""
        for j in range(8):
            hi += str(np.round(original_matrix[i][j][0], 4)) + " "
        print(hi)
    print("")
    
    for i in range(8):
        hi = ""
        for j in range(8):
            hi += str(np.round(dct_coefficients[i][j][0], 4)) + " "
        print(hi)
    print("")
    """
        
    return

main()