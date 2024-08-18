# Compiler to Use
CXX = g++

# Compiler Flags
CXXFLAGS = -Wall -Wextra -pedantic

SRC_DIR = src
BIN_DIR = bin
TEST_DIR = test

SHARED_HEADERS = $(SRC_DIR)/CommonTypes.hpp $(SRC_DIR)/JpegCompression.hpp

############# BITMAP AND TESTING #############

BITMAP_DECODER_SRC = $(SRC_DIR)/BitmapDecoder.cpp
BITMAP_DECODER_OBJECT = $(BIN_DIR)/BitmapDecoder.o

BITMAP_TEST_DIR = $(TEST_DIR)/bitmapTesting
BITMAP_TEST_SRC = $(BITMAP_TEST_DIR)/BitmapTester.cpp
BITMAP_TEST_OBJECT = $(BIN_DIR)/BitmapTester.o

# Exetuable path for bitmap testing
BITMAP_TEST_TARGET = $(BIN_DIR)/bitmapTest  

# Target that creates and run bitmap testing
bitmapTest: $(BITMAP_TEST_TARGET)
	./$(BITMAP_TEST_TARGET)

# Creating Bitmap Source Object file
$(BITMAP_DECODER_OBJECT): $(BITMAP_DECODER_SRC) $(SHARED_HEADERS)
	$(CXX) $(CXXFLAGS) -DBITMAP_DEBUG -o $(BITMAP_DECODER_OBJECT) -c $(BITMAP_DECODER_SRC)

# Creating Bitmap Test Object File
$(BITMAP_TEST_OBJECT): $(BITMAP_TEST_SRC)
	$(CXX) $(CXXFLAGS) -o $(BITMAP_TEST_OBJECT) -c $(BITMAP_TEST_SRC)

# Creates executable for bitmap testing at $(BITMAP_TEST_TARGET)
$(BITMAP_TEST_TARGET): $(BITMAP_DECODER_OBJECT) $(BITMAP_TEST_OBJECT)
	$(CXX) $(CXXFLAGS) -o $(BITMAP_TEST_TARGET) $(BITMAP_DECODER_OBJECT) $(BITMAP_TEST_OBJECT)

##############################################


############### DCT AND TESTING ##############

DCT_SRC = $(SRC_DIR)/DiscreteCosineTransformation.cpp
DCT_OBJECT = $(BIN_DIR)/DCT.o

DCT_TEST_DIR = $(TEST_DIR)/dctTesting
DCT_TEST_SRC = $(DCT_TEST_DIR)/DctTester.cpp
DCT_TEST_OBJECT = $(BIN_DIR)/DctTester.o

# Exetuable path for dct testing
DCT_TEST_TARGET = $(BIN_DIR)/dctTest 

# Target that creates and run DCT testing
dctTest: $(DCT_TEST_TARGET)
	./$(DCT_TEST_TARGET)

$(DCT_OBJECT): $(DCT_SRC) $(SHARED_HEADERS)
	$(CXX) $(CXXFLAGS) -o $(DCT_OBJECT) -c $(DCT_SRC)

$(DCT_TEST_OBJECT): $(DCT_TEST_SRC)
	$(CXX) $(CXXFLAGS) -o $(DCT_TEST_OBJECT) -c $(DCT_TEST_SRC)

# Creates executable for bitmap testing at $(DCT_TEST_TARGET)
$(DCT_TEST_TARGET): $(DCT_OBJECT) $(DCT_TEST_OBJECT)
	$(CXX) $(CXXFLAGS) -o $(DCT_TEST_TARGET) $(DCT_OBJECT) $(DCT_TEST_OBJECT)

##############################################

clean:
	rm -f $(BIN_DIR)/*

.PHONY: clean bitmapTest dctTest