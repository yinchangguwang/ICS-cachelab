#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "csim.h"
#include "cachelab.h"

// print usage info
void usage()
{
    printf("Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>\n");
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\n");
    printf("Examples:\n");
    printf("  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
    printf("  linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
    exit(1);
}

// parse command line and get the parameters
void parseline(int argc, char **argv)
{
    int opt;
    int num = 0;
    while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            usage();
            break;
        case 'v':
            verbose = 1;
            break;
        case 's':
            num = atoi(optarg);
            if (num == 0 && optarg[0] != '0')
            {
                printf("./csim: Missing required command line argument\n");
                usage();
            }
            numSet = num;
            break;
        case 'E':
            num = atoi(optarg);
            if (num == 0 && optarg[0] != '0')
            {
                printf("./csim: Missing required command line argument\n");
                usage();
            }
            associativity = num;
            break;
        case 'b':
            num = atoi(optarg);
            if (num == 0 && optarg[0] != '0')
            {
                printf("./csim: Missing required command line argument\n");
                usage();
            }
            blockSize = num;
            break;
        case 't':
            strcpy(filePath, optarg);
            break;
        case ':':
            printf("./csim: Missing required command line argument\n");
            usage();
            break;
        case '?':
            usage();
            break;
        default:
            printf("getopt error");
            exit(1);
            break;
        }
    }
}

// returns the line with the oldest age in the cache
int LRU(/*line **cacheArray, */int sets, int lines, int usedSet) {
	int r_line, r_age = -1;

	for (int j = 0; j < lines; j++) {
		if ((cacheArray[usedSet][j]).age > r_age) {
			r_age = (cacheArray[usedSet][j]).age;
			r_line = j;
		}
	}

	return r_line;
}

// increments the age for all cache lines except for 'work' one which get set to zero
void updateAge(/*line **cacheArray, */int sets, int lines, int usedSet, int usedLine) {
	//for (int i = 0; i < sets; i++) {
		for (int j = 0; j < lines; j++) {
			if (/*i == usedSet && */j == usedLine)
				(cacheArray[usedSet][j]).age = 0; //reset
			else 
				(cacheArray[usedSet][j]).age += 1; //increment

		}
	//}
}

void clean(/*line ** cacheArray, */int sets, int lines) {
	for(int i = 0; i < sets; i++) {
		free(cacheArray[i]);
	}
	free (cacheArray);
}

void Simulate(){
    // line **cacheArray;
    FILE *tracefile = fopen(filePath, "r");
    unsigned set, tag;   // used when working with address
    int bool_hit, bool_evict;   // true/false for hit and eviction
    int first_invalid_line, lru_line; // used to find space in cache to place fetched memory
    
    char operation;
    unsigned address;
    int size;

    // allocate space for an array of pointer to lines, sets element
	cacheArray = malloc(numSet * sizeof(line*));
    // allocate space for E lines in each set
	for (int i = 0; i < numSet; i++) {
		cacheArray[i] = malloc(associativity * sizeof(line));
	}

    // read in and 'act' on each line
    while(fscanf(tracefile, " %c %x,%d", &operation, &address, &size) > 0){
        bool_hit = 0;   // reset
        bool_evict = 0; // reset
        first_invalid_line = 0; // reset
		if (verbose) printf("%c %x,%d ", operation, address, size);
        if (operation == 'I'){
            if (verbose) printf("\n");
            continue;
        }
		set = (address / blockSize) % numSet; // get set from address
		tag = address / (numSet * blockSize); // get tag from address

        for(int i = 0; i < associativity; i++){
            // if the line is valid and the tag matches, it is a hit
			if ( (cacheArray[set][i]).valid == 1 && (cacheArray[set][i]).tag == tag ) {
				bool_hit = 1; // set hit boolean to true
				updateAge(/*cacheArray, */numSet, associativity, set, i); // // update ages of lines in cache (For LRU)
				break;
			}
			// if the line is invalid and  and it is the first invalid line, save it
			else if (!((cacheArray[set][i]).valid) && !first_invalid_line) { 
				first_invalid_line = i+1; // the (+1) is a fix use to be able to use the first_invalid_line as boolean and the actual line
			}
        }
        // if the cache lookup resulted in a miss, grab data from lower level cache and place in cache
		if (!bool_hit) {
			// if we found an invalid line, just use that one (= NO EVICTION)
			if (first_invalid_line) {
				bool_evict = 0;
				(cacheArray[set][first_invalid_line-1]).valid = 1; // set valid to true
				(cacheArray[set][first_invalid_line-1]).tag = tag; // set tag to match the one from requested address
				updateAge(/*cacheArray, */numSet, associativity, set, first_invalid_line-1); // update ages of lines in cache (For LRU)
			}
			// else set the LRU line (= EVICTION)
			else {
				bool_evict = 1;
				lru_line = LRU(/*cacheArray, */numSet, associativity, set);
				(cacheArray[set][lru_line]).valid = 1; // set valid to true
				(cacheArray[set][lru_line]).tag = tag; // set tag to match the one from requested address
				updateAge(/*cacheArray, */numSet, associativity, set, lru_line); // update ages of lines in cache (For LRU)
			}
		}
        // switch to update  hit_count, 
		switch (operation) {
			case 'M': // L + S
				if (bool_hit) {
					hits +=2;
					if (verbose) printf("hit hit\n");
				}
				else if (bool_evict) {
					misses +=1;
					evictions += 1;
	    			hits +=1;
					if (verbose) printf("miss eviction hit\n");
				}
				else {
					misses +=1;
					hits +=1;
					if (verbose) printf("miss hit\n");
				}
				break;

			case 'L':
				if (bool_hit) {
					hits +=1;
					if (verbose) printf("hit\n");
				}
				else if (bool_evict) {
					misses +=1;
					evictions += 1;
					if (verbose) printf("miss eviction\n");
				}
				else {
					misses +=1;
					if (verbose) printf("miss\n");
				}
				break;

			case 'S': 
				if (bool_hit) {
					hits +=1;
					if (verbose) printf("hit\n");
				}
				else if (bool_evict) {
					misses +=1;
					evictions += 1;
					if (verbose) printf("miss eviction\n");
				}
				else {
					misses +=1;
					if (verbose) printf("miss\n");
				}
				break;
		}
    }
    fclose(tracefile);
    clean(/*cacheArray, */numSet, associativity);
}

int main(int argc, char *argv[])
{
    parseline(argc, argv);
    Simulate();
    printSummary(hits, misses, evictions);
    return 0;
}
