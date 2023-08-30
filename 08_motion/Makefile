XX = g++
CXXFLAGS = -I.. -lX11 -lpthread -lpng -std=c++11

all: horn_schunck horn_schunck_multiscale lucas_kanade lucas_kanade_eigen cross_correlation phase_correlation kalman

horn_schunck: horn_schunck.cpp
	$(XX) -o $@ $^ $(CXXFLAGS)

horn_schunck_multiscale: horn_schunck_multiscale.cpp
	$(XX) -o $@ $^ $(CXXFLAGS)

lucas_kanade: lucas_kanade.cpp
	$(XX) -o $@ $^ $(CXXFLAGS)

lucas_kanade_eigen: lucas_kanade_eigen.cpp
	$(XX) -o $@ $^ $(CXXFLAGS)

cross_correlation: cross_correlation.cpp
	$(XX) -o $@ $^ $(CXXFLAGS)

phase_correlation: phase_correlation.cpp
	$(XX) -o $@ $^ $(CXXFLAGS)

kalman: kalman.cpp
	$(XX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -f horn_schunck
	rm -f horn_schunck_multiscale
	rm -f lucas_kanade
	rm -f lucas_kanade_eigen
	rm -f cross_correlation
	rm -f phase_correlation
	rm -f kalman