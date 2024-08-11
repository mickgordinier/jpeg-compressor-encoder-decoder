from PIL import Image
import numpy as np

img = Image.open("test_img/test_10.bmp")
print(np.array(img)[:,:,0], "\n")
print(np.array(img)[:,:,1], "\n")
print(np.array(img)[:,:,2], "\n")

with open("test_img/test_10.bmp", "rb") as image:
  f = image.read()
  b = bytearray(f)
  print(b)

# BMP Header  (14 bytes total)
# Bytes 0-1 are BMP signature == "BM"  (2 bytes)
sign = b[0:2]
file_size = int.from_bytes(b[2:6], "little")      # size of BMP file in bytes (2 bytes)
# Bytes 6-9 unused (4 bytes)
data_offset = int.from_bytes(b[10:14], "little")   # offset from beginning of file to the beginning of the bitmap data

# BMP Info Header (40 bytes total)
info_header_size = int.from_bytes(b[14:18], "little")
width = int.from_bytes(b[18:22], "little")
height = int.from_bytes(b[22:26], "little")
planes = int.from_bytes(b[26:28], "little")
bits_per_pixel = int.from_bytes(b[28:30], "little")
compression_type = int.from_bytes(b[30:34], "little")
compressed_image_size = int.from_bytes(b[34:38], "little")
x_ppm = int.from_bytes(b[38:42], "little")
y_ppm = int.from_bytes(b[42:46], "little")
colors_used = int.from_bytes(b[46:50], "little")
important_colors = int.from_bytes(b[50:54], "little")

# BMP Color Table (IGNORED)

# BMP Pixel Data (Should be of length "compressed_image_size")
# Read in bottom-up (as shown in browsers) and from left to right
# Also need to account for read in as BGR instead of RGB
img_matrix = np.zeros((height,width,3))


for rowIdx in range(14):
  for colIdx in range(14):
    byte_addr = 54 + rowIdx*44 + colIdx*3
print(byte_addr)



  
"""
# JFIF Header
soi = b[0:2]            # Start of Image Marker (Will be FF D8 for JPEG) (2 bytes)
app0_marker = b[2:4]    # app0 marker (Will be FF E0 for JFIF and FF E1 for EXIF) (2 bytes)
app0_length = b[4:6]    # app0 length including this (Will be 0x00 10 == 16) (2 bytes)
identifier = b[6:11]    # Will be JFIF with null terminator (5 bytes)
version = b[11:13]      # JFIF version. byte split by '.' (Ex. 0x01 01 == Version 1.1) (2 bytes)
units = b[13]           # Unit for X,Y densities (0 = no units, 1 = dpi, 2 = dpcm)
x_density = b[14:16]
y_density = b[16:18]
x_thumbnail = b[18]
y_thumbnail = b[19]

print(b[20:22])
"""