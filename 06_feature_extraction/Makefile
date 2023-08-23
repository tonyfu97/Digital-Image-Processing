CXX = g++
CXXFLAGS = -I.. -lX11 -lpthread -lpng

all: harris shi_tomasi

harris: harris.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

shi_tomasi: shi_tomasi.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f harris
	rm -f shi_tomasi