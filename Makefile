CFLAGS = --std=c99 -g -Wall --pedantic
LIBS = -lm

CC = gcc

all:
	${CC} -o test_marray test_marray.c $(CFLAGS) $(LIBS)

clean:
	rm -rf test_marray
