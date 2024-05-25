#pragma once

#define MININT -2147483648

// cache parameters
int numSet;         //s
int associativity;  //E
int blockSize;      //b
char filePath[100]; //t
int verbose = 0;

typedef struct _line{
	int valid; // valid bit equivalent in line
	unsigned long int tag; // tag to identify match for address
	int age;
} line;

line **cacheArray;

// final results
int hits = 0;
int misses = 0;
int evictions = 0;

// will be set in getopt() function
extern char *optarg;

// you can define functions here
void usage();
void parseline(int argc, char **argv);
void Simulate();
int LRU(/*line **cacheArray, */int sets, int lines, int usedSet);
void clean(/*line ** cacheArray, */int sets, int lines);
void updateAge(/*line **cacheArray, */int sets, int lines, int usedSet, int usedLine);