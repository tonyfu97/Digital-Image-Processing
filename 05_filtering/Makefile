CXX = g++
CXXFLAGS = -I.. -lX11 -lpthread -lpng

all: convolution median derivative second_derivatives sigma_filter adaptive_window deriche fft gaussian

convolution: convolution.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

median: median.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

derivative: derivative.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

second_derivatives: second_derivatives.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

sigma_filter: sigma_filter.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

adaptive_window: adaptive_window.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

deriche: deriche.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

fft: fft.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

gaussian: gaussian.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f convolution
	rm -f median
	rm -f derivative
	rm -f second_derivatives
	rm -f sigma_filter
	rm -f adaptive_window
	rm -f deriche
	rm -f fft
	rm -f gaussian