BitmapDecoder.o: BitmapDecoder.cpp BitmapDecoder.hpp
	g++ -c BitmapDecoder.cpp

BitmapTester.o: testing/bitmap_testing/BitmapTester.cpp
	g++ -c testing/bitmap_testing/BitmapTester.cpp

# Creates performs testing for Bitmap images
bitmap_test: BitmapDecoder.o BitmapTester.o
	g++ -o bitmap_test BitmapDecoder.o BitmapTester.o
	./bitmap_test


Discrete_Cosine_Transformation.o: Discrete_Cosine_Transformation.cpp Discrete_Cosine_Transformation.hpp
	g++ -c Discrete_Cosine_Transformation.cpp

DCT_Tester.o: testing/dct_testing/DCT_Tester.cpp
	g++ -c testing/dct_testing/DCT_Tester.cpp

# Creates performs testing for Bitmap images
dct_test: Discrete_Cosine_Transformation.o DCT_Tester.o
	g++ -o dct_test Discrete_Cosine_Transformation.o DCT_Tester.o
	./dct_test


.PHONY: clean
clean:
	rm -f *.o *.out *_test 