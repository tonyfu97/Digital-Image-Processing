CXX = g++
CXXFLAGS = -I.. -lX11 -lpthread -lpng

all: block_decomposition

block_decomposition: block_decomposition.cpp ../CImg.h
	$(CXX) $(CXXFLAGS) block_decomposition.cpp -o block_decomposition

clean:
	rm -f block_decomposition