# Compiler to Use
CXX = g++

# Compiler Flags
CXXFLAGS = -Wall -Wextra -pedantic

# Defaulting Debug to be 0
DEBUG ?= 0

# IF debug declared, then include debug macros
ifeq ($(DEBUG), 1)
    CXXFLAGS += -DMAIN_DEBUG -DBITMAP_DEBUG
endif

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = test

SHARED_HEADERS = $(SRC_DIR)/CommonTypes.hpp $(SRC_DIR)/JpegCompression.hpp

all: bitmapTest dctTest compressedBitmap 

############### COMMON RULES ###############

# WARNING: THIS MEANS THAT WE MUST ENSURE EACH FILENAME IS DIFFERENT

# Creates individual object files based on each source file required
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(SHARED_HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

# Creates individual object files based on each test file required
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp $(SHARED_HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

# Creates individual object files based on each main file required
$(OBJ_DIR)/%.o: %.cpp $(SHARED_HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

#############################################

# ################ BITMAP TESTING #############

# Indicating object files used to create executable
BITMAP_TESTER_OBJS = $(OBJ_DIR)/BitmapDecoder.o $(OBJ_DIR)/BitmapTester.o

# Exetuable path for bitmap testing
BITMAP_TEST_TARGET = $(BIN_DIR)/bitmapTest  

# Target that creates bitmap testing
bitmapTest: $(BITMAP_TEST_TARGET)

# Creates executable for bitmap testing at $(BITMAP_TEST_TARGET)
$(BITMAP_TEST_TARGET): $(BITMAP_TESTER_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# ##############################################


# ############### DCT AND TESTING ##############

DCT_TEST_OBJECT = $(OBJ_DIR)/DiscreteCosineTransformation.o $(OBJ_DIR)/DctTester.o

# Exetuable path for dct testing
DCT_TEST_TARGET = $(BIN_DIR)/dctTest 

# Target that creates and run DCT testing
dctTest: $(DCT_TEST_TARGET)

# Creates executable for bitmap testing at $(DCT_TEST_TARGET)
$(DCT_TEST_TARGET): $(DCT_TEST_OBJECT)
	$(CXX) $(CXXFLAGS) -o $@ $^

# ##############################################

######### CREATING COMPRESSED BITMAP #########

# Converts all src files above into object files in the obj directory
COMPRESSED_BITMAP_OBJS = $(OBJ_DIR)/BitmapDecoder.o \
												$(OBJ_DIR)/DiscreteCosineTransformation.o \
												$(OBJ_DIR)/Quantization.o \
												$(OBJ_DIR)/YCbCrTransformation.o \
												$(OBJ_DIR)/createCompressedBitmap.o

# Indicating path to executable
COMPRESSED_BITMAP_TARGET = $(BIN_DIR)/compressedBitmap

# Creating target that produces compressed bitmap executable in bin
compressedBitmap: $(COMPRESSED_BITMAP_TARGET)

# Creates executable for producing compressed bitmap image
$(COMPRESSED_BITMAP_TARGET): $(COMPRESSED_BITMAP_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

##############################################

clean:
	rm -f $(BIN_DIR)/* $(OBJ_DIR)/*

.PHONY: clean all bitmapTest dctTest compressedBitmap 