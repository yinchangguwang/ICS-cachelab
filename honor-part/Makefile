#
# Student makefile for Cache Lab
# Note: requires a 64-bit x86-64 system 
#
CC = gcc
CFLAGS = -g -Wall -Werror -std=c99 -m64

all: test-mul tracegen

test-mul: test-mul.c mul.o cachelab.c cachelab.h
	$(CC) $(CFLAGS) -o test-mul test-mul.c cachelab.c mul.o 

tracegen: tracegen.c mul.o cachelab.c
	$(CC) $(CFLAGS) -O0 -o tracegen tracegen.c mul.o cachelab.c

mul.o: mul.c
	$(CC) $(CFLAGS) -O0 -c mul.c

# mul.o: mul.c
# 	$(CC) $(CFLAGS) -O0 -c mul.c

#
# Clean the src dirctory
#
clean:
	rm -rf *.o
	rm -f *.tar
	rm -f test-mul tracegen
	rm -f trace.all trace.f*
	rm -f .csim_results .marker
