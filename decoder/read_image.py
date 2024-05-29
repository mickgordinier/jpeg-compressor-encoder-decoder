import numpy as np

with open("Flower3.jpg", "rb") as image:
  f = image.read()
  b = bytearray(f)

# Start of Image
print("\nSOI Marker: " + str(b[0:2]) + "\n")  # SOI (Start of Image Marker) = 0xFFD8


# Application 0 (Default Header)
print("APP0 Marker: " + str(b[2:4]))  # APPO Marker = 0xFFE0
print("APP0 Length: " + str(int.from_bytes(b[4:6], "big")))

# APP0 Identifier String length up until "\0" (null)
byte_idx =6
while(b[byte_idx]):
    byte_idx = byte_idx + 1

print("APP0 Identifier: " + str(b[6:byte_idx]))
print("APP0 Version: " + str(b[byte_idx+1]) + "." + str(b[byte_idx+2]))
print("APP0 Units: " + str(int.from_bytes(b[byte_idx+3:byte_idx+4], "big")))
print("APP0 Density: " + str(int.from_bytes(b[byte_idx+4:byte_idx+6], "big")) + " x " + str(int.from_bytes(b[byte_idx+6:byte_idx+8], "big")))
print("APP0 Thumbnail: " + str(int.from_bytes(b[byte_idx+8:byte_idx+9], "big")) + " x " + str(int.from_bytes(b[byte_idx+9:byte_idx+10], "big")))


# Skipping all unneeded APP1-APPF
byte_idx = byte_idx + 10

while ((int.from_bytes(b[byte_idx+1:byte_idx+2], "big") & 0xF0) == 0xE0):
    print("\nAPPx Marker: " + str(b[byte_idx:byte_idx+2]))
    print("APPx Length: " + str(int.from_bytes(b[byte_idx+2:byte_idx+4], "big")))
    byte_idx = byte_idx + int.from_bytes(b[byte_idx+2:byte_idx+4], "big") + 2


# Reading in the Quantization Matrices
print("\nQT 1 Marker: " + str(b[byte_idx:byte_idx+2]))
print("QT 1 Length: " + str(int.from_bytes(b[byte_idx+2:byte_idx+4], "big")))
print("QT 1 Destination: " + str(int.from_bytes(b[byte_idx+4:byte_idx+5], "big")))

byte_idx = byte_idx+5
quantization_matrix_1 = np.zeros((8,8))
for i in range(8):
    for j in range(8):
        quantization_matrix_1[i][j] = int.from_bytes(b[byte_idx:byte_idx+1], "big")
        byte_idx = byte_idx+1
print(quantization_matrix_1)

print("\nQT 2 Marker: " + str(b[byte_idx:byte_idx+2]))
print("QT 2 Length: " + str(int.from_bytes(b[byte_idx+2:byte_idx+4], "big")))
print("QT 2 Destination: " + str(int.from_bytes(b[byte_idx+4:byte_idx+5], "big")))

byte_idx = byte_idx+5
quantization_matrix_2 = np.zeros((8,8))
for i in range(8):
    for j in range(8):
        quantization_matrix_2[i][j] = int.from_bytes(b[byte_idx:byte_idx+1], "big")
        byte_idx = byte_idx+1
print(quantization_matrix_2)

print("\nStart of Frame (SOF) Marker: " + str(b[byte_idx:byte_idx+2]))
print("SOF Length: " + str(int.from_bytes(b[byte_idx+2:byte_idx+4], "big")))
print("SOF Precision: " + str(int.from_bytes(b[byte_idx+4:byte_idx+5], "big")))
print("SOF Height: " + str(int.from_bytes(b[byte_idx+5:byte_idx+7], "big")))
print("SOF Width: " + str(int.from_bytes(b[byte_idx+7:byte_idx+9], "big")))

num_components = int.from_bytes(b[byte_idx+9:byte_idx+10], "big")
print("Number of Components: " + str(num_components))

byte_idx = byte_idx + 10
for i in range(num_components):
    print("Component " + str(i) + " ID: " + str(int.from_bytes(b[byte_idx:byte_idx+1], "big")))
    print("Component " + str(i) + " Sampling Factor: " + str(int.from_bytes(b[byte_idx+1:byte_idx+2], "big")))
    print("Component " + str(i) + " QT Idx: " + str(int.from_bytes(b[byte_idx+2:byte_idx+3], "big")))
    byte_idx = byte_idx + 3
    
print("\nHuffman Table Marker: " + str(b[byte_idx:byte_idx+2]))
print("Huffman Table Length: " + str(int.from_bytes(b[byte_idx+2:byte_idx+4], "big")))
print("Huffman Table Class: " + str(int.from_bytes(b[byte_idx+4:byte_idx+5], "big") >> 4))
print("Huffman Table Dest Identifier: " + str(int.from_bytes(b[byte_idx+4:byte_idx+5], "big") & 0xF))

byte_idx = byte_idx + 5
num_code_per_length_table = []
for i in range(16):
    num_code_per_length_table.append(int.from_bytes(b[byte_idx+i:byte_idx+i+1], "big"))

byte_idx = byte_idx + 16
huffman_dict_1 = {}
for i in range(16):
    continue
print(num_code_per_length_table)