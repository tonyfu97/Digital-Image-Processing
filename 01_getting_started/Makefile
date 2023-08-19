CXX = g++
CXXFLAGS = -I.. -lX11 -lpthread -lpng

all: first_code

first_code: first_code.cpp ../CImg.h
	$(CXX) $(CXXFLAGS) first_code.cpp -o first_code

clean:
	rm -f first_code