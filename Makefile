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

BITMAP_TEST_DIR = $(TEST_DIR)/bitmap_testing
BITMAP_TEST_SRC = $(BITMAP_TEST_DIR)/BitmapTester.cpp
BITMAP_TEST_OBJECT = $(BIN_DIR)/BitmapTester.o

# Exetuable path for bitmap testing
BITMAP_TEST_TARGET = ./$(BIN_DIR)/bitmap_test  

# Target that creates and run bitmap testing
bitmap_test: $(BITMAP_TEST_TARGET)
	cd $(BIN_DIR)
	./$(BITMAP_TEST_TARGET)
	cd ..

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

Discrete_Cosine_Transformation.o: Discrete_Cosine_Transformation.cpp Discrete_Cosine_Transformation.hpp
	g++ -c Discrete_Cosine_Transformation.cpp

DCT_Tester.o: testing/dct_testing/DCT_Tester.cpp
	g++ -c testing/dct_testing/DCT_Tester.cpp

# Creates performs testing for Bitmap images
dct_test: Discrete_Cosine_Transformation.o DCT_Tester.o
	g++ -o dct_test Discrete_Cosine_Transformation.o DCT_Tester.o
	./dct_test

.PHONY: clean debug
clean:
	rm -f *.o $(BIN_DIR)/*.o $(BIN_DIR)/*_test 