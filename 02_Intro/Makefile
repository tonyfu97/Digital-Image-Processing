CXX = g++
CXXFLAGS = -I.. -lX11 -lpthread -lpng

all: first_code block_decomposition

first_code: first_code.cpp ../CImg.h
	$(CXX) $(CXXFLAGS) first_code.cpp -o first_code

block_decomposition: block_decomposition.cpp ../CImg.h
	$(CXX) $(CXXFLAGS) block_decomposition.cpp -o block_decomposition

clean:
	rm -f first_code
	rm -f block_decomposition