CXX = g++
CXXFLAGS = -I.. -lX11 -lpthread -lpng

all: erosion_dilation open_close kramer_bruckner asf

erosion_dilation: erosion_dilation.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS)

open_close: open_close.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS)

kramer_bruckner: kramer_bruckner.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS)

asf: asf.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -f erosion_dilation
	rm -f open_close
	rm -f kramer_bruckner
	rm -f asf