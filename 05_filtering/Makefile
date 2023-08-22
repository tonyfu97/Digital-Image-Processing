CXX = g++
CXXFLAGS = -I.. -lX11 -lpthread -lpng

all: convolution median derivative second_derivatives

convolution: convolution.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

median: median.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

derivative: derivative.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

second_derivatives: second_derivatives.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f convolution
	rm -f median
	rm -f derivative
	rm -f second_derivatives