CXX = g++
CXXFLAGS = -I.. -lX11 -lpthread -lpng -std=c++11

all: harris shi_tomasi hough hough_circle texture_spectrum tamura_contrast tamura_coarseness tamura_directionality lbp

harris: harris.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

shi_tomasi: shi_tomasi.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

hough: hough.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

hough_circle: hough_circle.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

texture_spectrum: texture_spectrum.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

tamura_contrast: tamura_contrast.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

tamura_coarseness: tamura_coarseness.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

tamura_directionality: tamura_directionality.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

lbp: lbp.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f harris
	rm -f shi_tomasi
	rm -f hough
	rm -f hough_circle
	rm -f texture_spectrum
	rm -f tamura_contrast
	rm -f tamura_coarseness
	rm -f tamura_directionality
	rm -f lbp