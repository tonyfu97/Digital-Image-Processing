CXX = g++
CXXFLAGS = -I.. -lX11 -lpthread -lpng

all: convolution median

convolution: convolution.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

median: median.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f convolution
	rm -f median