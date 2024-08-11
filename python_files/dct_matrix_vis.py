import numpy as np

# DCT Matrix
matrix_size = 8
dctMatrix = np.zeros((matrix_size,matrix_size))

dctMatrix[0,:] = np.sqrt(1/matrix_size)

for row in range(matrix_size):
    for col in range(matrix_size):
        dctMatrix[row][col] = (np.sqrt(2/matrix_size) * (2*col + 1) * row * np.pi) / (2 * matrix_size)
        
print(dctMatrix)


# DCT Basis Image
dct_basis_matrix = np.zeros((matrix_size**2, matrix_size**2))
