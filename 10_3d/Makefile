XX = g++
CXXFLAGS = -I.. -lX11 -lpthread -lpng -std=c++11

all: simple_3d

simple_3d: simple_3d.cpp
	$(XX) $(CXXFLAGS) -o simple_3d simple_3d.cpp

clean:
	rm -f simple_3d