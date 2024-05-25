/*
 * cachelab.c - Cache Lab helper functions
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cachelab.h"
#include <time.h>

mul_func_t func_list[MAX_MUL_FUNCS];
int func_counter = 0;

/*
 * printSummary - Summarize the cache simulation statistics. Student cache simulators
 *                must call this function in order to be properly autograded.
 */
void printSummary(int hits, int misses, int evictions)
{
    printf("hits:%d misses:%d evictions:%d\n", hits, misses, evictions);
    FILE *output_fp = fopen(".csim_results", "w");
    assert(output_fp);
    fprintf(output_fp, "%d %d %d\n", hits, misses, evictions);
    fclose(output_fp);
}

/*
 * initMatrix - Initialize the given matrix
 */
void initMatrix(int M, int N, int A[N][M], int B[M][N], int C[N][N])
{
    randMatrix(M, N, A);
    randMatrix(N, M, B);
    correctMul(M, N, A, B, C);
}

void randMatrix(int M, int N, int A[N][M])
{
    int i, j;
    srand(time(NULL));
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; j++)
        {
            // A[i][j] = i+j;  /* The matrix created this way is symmetric */
            A[i][j] = rand() % 100;
        }
    }
}

/*
 * correctMul - baseline multiply function used to evaluate correctness
 */
void correctMul(int M, int N, int A[N][M], int B[M][N], int C[N][N])
{
    int i, j, k, tmp;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            tmp = 0;
            for (k = 0; k < M; k++)
            {
                tmp += A[i][k] * B[k][j];
            }
            C[i][j] = tmp;
        }
    }
}

/*
 * registerMulFunction - Add the given multiply function into your list
 *     of functions to be tested
 */
void registerMulFunction(void (*mul)(int M, int N, int[N][M], int[M][N], int[N][N]),
                         char *desc)
{
    func_list[func_counter].func_ptr = mul;
    func_list[func_counter].description = desc;
    func_list[func_counter].correct = 0;
    func_list[func_counter].num_hits = 0;
    func_list[func_counter].num_misses = 0;
    func_list[func_counter].num_evictions = 0;
    func_counter++;
}
