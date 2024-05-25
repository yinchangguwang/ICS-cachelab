/*
 * tracegen.c - Running the binary tracegen with valgrind produces
 * a memory trace of all of the registered multiply functions.
 *
 * The beginning and end of each registered multiply function's trace
 * is indicated by reading from "marker" addresses. These two marker
 * addresses are recorded in file for later use.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include "cachelab.h"
#include <string.h>

/* External variables declared in cachelab.c */
extern mul_func_t func_list[MAX_MUL_FUNCS];
extern int func_counter;

/* External function from muls.c */
extern void registerFunctions();

/* Markers used to bound trace regions of interest */
volatile char MARKER_START, MARKER_END;

static int A[256][256];
static int B[256][256];
static int C[256][256];
static int M;
static int N;

int validate(int fn, int M, int N, int A[N][M], int B[M][N], int C[N][N])
{
    int rC[N][N];
    memset(rC, 0, sizeof(rC));
    correctMul(M, N, A, B, rC);
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (rC[i][j] != C[i][j])
            {
                printf("Validation failed on function %d! Expected %d but got %d at C[%d][%d]\n", fn, rC[i][j], C[i][j], i, j);
                return 0;
            }
        }
    }
    return 1;
}

int main(int argc, char *argv[])
{
    int i;

    char c;
    int selectedFunc = -1;
    while ((c = getopt(argc, argv, "M:N:F:")) != -1)
    {
        switch (c)
        {
        case 'M':
            M = atoi(optarg);
            break;
        case 'N':
            N = atoi(optarg);
            break;
        case 'F':
            selectedFunc = atoi(optarg);
            break;
        case '?':
        default:
            printf("./tracegen failed to parse its options.\n");
            exit(1);
        }
    }

    /*  Register multiply functions */
    registerFunctions();

    /* Fill A with data */
    initMatrix(M, N, A, B, C);

    /* Record marker addresses */
    FILE *marker_fp = fopen(".marker", "w");
    assert(marker_fp);
    fprintf(marker_fp, "%llx %llx",
            (unsigned long long int)&MARKER_START,
            (unsigned long long int)&MARKER_END);
    fclose(marker_fp);

    if (-1 == selectedFunc)
    {
        /* Invoke registered multiply functions */
        for (i = 0; i < func_counter; i++)
        {
            MARKER_START = 33;
            (*func_list[i].func_ptr)(M, N, A, B, C);
            MARKER_END = 34;
            if (!validate(i, M, N, A, B, C))
                return i + 1;
        }
    }
    else
    {
        MARKER_START = 33;
        (*func_list[selectedFunc].func_ptr)(M, N, A, B, C);
        MARKER_END = 34;
        if (!validate(selectedFunc, M, N, A, B, C))
            return selectedFunc + 1;
    }
    return 0;
}
