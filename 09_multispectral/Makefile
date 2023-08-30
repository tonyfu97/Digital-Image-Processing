XX = g++
CXXFLAGS = -I.. -lX11 -lpthread -lpng -std=c++11

all: pca median_color

pca: pca.cpp
	$(XX) $(CXXFLAGS) -o pca pca.cpp

median_color: median_color.cpp
	$(XX) $(CXXFLAGS) -o median_color median_color.cpp

clean:
	rm -f pca
	rm -f median_color