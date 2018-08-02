PROG = ships.cpp
CXX = g++ -std=c++17
LDFLAGS = -lX11 -lxtgeo

all: | main run

main: | geometry $(PROG)
	$(CXX) -o main $(PROG) $(LDFLAGS)

geometry: $(wildcard geometry/*.?pp)
	$(MAKE) -C geometry

run:
	./main
