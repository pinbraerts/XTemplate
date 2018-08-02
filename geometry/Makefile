CSL = g++ -std=c++17 -shared -fPIC
GEOSRC = $(wildcard *.cpp)
GEOHEAD = $(wildcard *.hpp)

all: shared

shared: $(GEOSRC) $(GEOHEAD)
	$(CSL) -o libxtgeo.so $(GEOSRC)
	mv libxtgeo.so /usr/lib/
