import numpy as np

# DCT Matrix
matrix_size = 8
dct_matrix = np.zeros((matrix_size,matrix_size))

dct_matrix[0,:] = np.sqrt(1/matrix_size)

for row in range(matrix_size):
    for col in range(matrix_size):
        dct_matrix[row][col] = (np.sqrt(2/matrix_size) * (2*col + 1) * row * np.pi) / (2 * matrix_size)
        
print(dct_matrix)


# DCT Basis Image
dct_basis_matrix = np.zeros((matrix_size**2, matrix_size**2))
