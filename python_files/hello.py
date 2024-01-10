with open("img1.jpg", "rb") as image:
  f = image.read()
  b = bytearray(f)
  
  
  print(len(b))
  print(b[:21])
  

# Important for disceting certain types of JPEGS
# Link 1: https://www.file-recovery.com/jpg-signature-format.htm
# Link 2: https://www.garykessler.net/library/file_sigs.html

class JPG_File_Header:
    def __init__(self, img_bytes):
        soi = b[0:2]  # Start of Image Marker -- ALWAY 'FF D8' for JPEG
        app0 = b[2:4]  # Application Use Marker -- 'FF E0' for Standard JPEG/JFIF file ;  'FF E1' for Standard JPEG file with Exif metadata
         