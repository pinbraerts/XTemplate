.POSIX:

CXX = g++ -std=c++17
LDFLAGS = -lX11 -lxtgeo -Lgeometry

all: ships

ships: geometry ships.cpp
	$(CXX) -o ships ships.cpp $(LDFLAGS)

geometry: $(wildcard geometry/*.?pp)
	$(MAKE) -C geometry

run: ships
	LD_LIBRARY_PATH=geometry ./ships
