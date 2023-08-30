XX = g++
CXXFLAGS = -I.. -lX11 -lpthread -lpng -std=c++11

all: pca

pca: pca.cpp
	$(XX) $(CXXFLAGS) -o pca pca.cpp

clean:
	rm -f pca