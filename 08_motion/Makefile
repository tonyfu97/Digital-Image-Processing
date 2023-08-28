XX = g++
CXXFLAGS = -I.. -lX11 -lpthread -lpng -std=c++11

all: horn_schunck

horn_schunck: horn_schunck.cpp
	$(XX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -f horn_schunck