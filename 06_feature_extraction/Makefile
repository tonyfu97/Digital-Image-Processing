CXX = g++
CXXFLAGS = -I.. -lX11 -lpthread -lpng

all: harris

harris: harris.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f harris