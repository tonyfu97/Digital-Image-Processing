CXX = g++
CXXFLAGS = -I.. -lX11 -lpthread -lpng

all: erosion_dilation

erosion_dilation: erosion_dilation.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -f erosion_dilation