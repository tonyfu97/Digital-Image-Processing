CXX = g++
CXXFLAGS = -I.. -lX11 -lpthread -lpng

all: harris shi_tomasi hough hough_circle texture_spectrum

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

clean:
	rm -f harris
	rm -f shi_tomasi
	rm -f hough
	rm -f hough_circle
	rm -f texture_spectrum