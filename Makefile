PROG = ships.cpp
OPTS = -lX11

all: main run

main:
	g++ -o main $PROG $OPTS

run:
	./main
