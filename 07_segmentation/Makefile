XX = g++
CXXFLAGS = -I.. -lX11 -lpthread -lpng -std=c++11

all: active_contours otsu bernsen

active_contours: active_contours.cpp
	$(XX) -o $@ $^ $(CXXFLAGS)

otsu: otsu.cpp
	$(XX) -o $@ $^ $(CXXFLAGS)

bernsen: bernsen.cpp
	$(XX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -f active_contours
	rm -f otsu
	rm -f bernsen