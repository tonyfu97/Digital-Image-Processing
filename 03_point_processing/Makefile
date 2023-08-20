CXX = g++
CXXFLAGS = -I.. -lX11 -lpthread -lpng

all: transformations bitwise

transformations: transformations.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS)

bitwise: bitwise.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -f transformations
	rm -f bitwise