/*
 * mul.c - Matrix multiply C = A * B
 *
 * Each multiply function must have a prototype of the form:
 * void mul(int M, int N, int A[N][M], int B[M][N], int C[N][N]);
 *
 * A multiply function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"

int is_mul(int M, int N, int A[N][M], int B[M][N], int C[N][N]);

/*
 * multiply_submit - This is the solution multiply function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "multiply submission", as the driver
 *     searches for that string to identify the multiply function to
 *     be graded.
 */

char mul_submit_desc[] = "multiply submission";
void mul_submit(int M, int N, int A[N][M], int B[M][N], int C[N][N])
{
    // 笑嘻了矩阵C初值设置的就是答案，不归零就两倍了，交个空函数都能过
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            C[i][j] = 0;
        }
    }

    for(int k = 0; k < 32; k += 8){
        for(int ai = 0; ai < 32; ai += 8){
            for(int bj = 0; bj < 32; bj += 8){
                for(int ia = 0; ia < 8; ia++){
                    int tmp1[8] = {0, 0, 0, 0, 0, 0, 0, 0};
                    int tmp2[8] = {0, 0, 0, 0, 0, 0, 0, 0};
                    for(int ja = 0; ja < 8; ja++){
                        tmp1[ja] = A[ai + ia][k + ja];
                    }
                    for(int jb = 0; jb < 8; jb++){
                        for(int ib = 0; ib < 8; ib++){
                            tmp2[jb] += tmp1[ib] * B[k + ib][bj + jb];
                        }
                    }
                    for(int idx = 0; idx < 8; idx++){
                        C[ai + ia][bj + idx] += tmp2[idx];
                    }
                }
            }
        }
    }
}

/*
 * mul - A simple multiply function, not optimized for the cache.
 */
char mul_desc[] = "multiply submission";
void mul(int M, int N, int A[N][M], int B[M][N], int C[N][N])
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
 * registerFunctions - This function registers your multiply
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     multiply strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerMulFunction(mul_submit, mul_submit_desc);

    /* Register any additional multiply functions */
    // registerMulFunction(mul, mul_desc);
}

/*
 * is_multiply - This helper function checks if C is the multiply of
 *     A and B. You can check the correctness of your multiply by calling
 *     it before returning from the multiply function.
 */
int is_mul(int M, int N, int A[N][M], int B[M][N], int C[N][N])
{
    int i, j, k;
    int num = 0;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            num = 0;
            for (k = 0; k < M; k++)
            {
                num += A[i][k] * B[k][j];
            }
            if (num != C[i][j])
            {
                return 0;
            }
        }
    }
    return 1;
}
