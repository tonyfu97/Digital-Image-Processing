XX = g++
CXXFLAGS = -I.. -lX11 -lpthread -lpng -std=c++11

all: jpeg

jpeg: jpeg.cpp
	$(XX) $(CXXFLAGS) -o jpeg jpeg.cpp

clean:
	rm -f 