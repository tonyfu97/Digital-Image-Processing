CXX = g++
CXXFLAGS = -I.. -lX11 -lpthread -lpng

all: fill_example

fill_example: fill_example.cpp ../CImg.h
	$(CXX) $(CXXFLAGS) fill_example.cpp -o fill_example

clean:
	rm -f fill_example