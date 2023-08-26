XX = g++
CXXFLAGS = -I.. -lX11 -lpthread -lpng -std=c++11

all: active_contours

active_contours: active_contours.cpp
	$(XX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -f active_contours