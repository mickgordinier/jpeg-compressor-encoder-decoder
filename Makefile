BitmapDecoder.o: BitmapDecoder.cpp BitmapDecoder.hpp
	g++ -c BitmapDecoder.cpp

BitmapTester.o: testing/BitmapTester.cpp
	g++ -c testing/BitmapTester.cpp

# Creates performs testing for Bitmap images
bitmap_test: BitmapDecoder.o BitmapTester.o
	g++ -o bitmap_test BitmapDecoder.o BitmapTester.o
	./bitmap_test

.PHONY: clean
clean:
	rm -f *.o *.out bitmap_test