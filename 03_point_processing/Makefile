CXX = g++
CXXFLAGS = -I.. -lX11 -lpthread -lpng

all: transformations bitwise histogram_equalization

transformations: transformations.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS)

bitwise: bitwise.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS)

histogram_equalization: histogram_equalization.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -f transformations
	rm -f bitwise
	rm -f histogram_equalization