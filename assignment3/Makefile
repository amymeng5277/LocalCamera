CC=gcc -Wall
all : a3-ece650.o a2-ece650.o rgen.o
	$(CC) -c rgen rgen.c
	$(CC) -c a2-ece650 a2-ece650.c
	$(CC) -c a3-ece650 a3-ece650.c
rgen.o : rgen.c rgen.h base.h base.c util.c util.h
	$(CC) -c rgen.c

clean:
	 rm -f *.orgen a3-ece650 a2-ece650 rgen
