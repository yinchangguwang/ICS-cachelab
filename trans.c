/*
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/*
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded.
 */
void trans48x48(int M, int N, int A[N][M], int B[M][N]);
void trans96x96(int M, int N, int A[N][M], int B[M][N]);
void trans93x99(int M, int N, int A[N][M], int B[M][N]);

char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    switch (M)
    {
    case (48):
        trans48x48(M, N, A, B);
        break;
    case (96):
        trans96x96(M, N, A, B);
        break;
    case (93):
        trans93x99(M, N, A, B);
        break;
    default:
        break;
    }
}

void trans48x48(int M, int N, int A[N][M], int B[M][N]){
    int t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11;
    for(int k = 0; k < 16; k++){
        for(int i = 0; i < 12; i++){
            // shit
            t0 = A[k / 4 * 12 + i][k % 4 * 12 + 0];
            t1 = A[k / 4 * 12 + i][k % 4 * 12 + 1];
            t2 = A[k / 4 * 12 + i][k % 4 * 12 + 2];
            t3 = A[k / 4 * 12 + i][k % 4 * 12 + 3];
            t4 = A[k / 4 * 12 + i][k % 4 * 12 + 4];
            t5 = A[k / 4 * 12 + i][k % 4 * 12 + 5];
            t6 = A[k / 4 * 12 + i][k % 4 * 12 + 6];
            t7 = A[k / 4 * 12 + i][k % 4 * 12 + 7];
            t8 = A[k / 4 * 12 + i][k % 4 * 12 + 8];
            t9 = A[k / 4 * 12 + i][k % 4 * 12 + 9];
            t10 = A[k / 4 * 12 + i][k % 4 * 12 + 10];
            t11 = A[k / 4 * 12 + i][k % 4 * 12 + 11];

            B[k % 4 * 12 + 0][k / 4 * 12 + i] = t0;
            B[k % 4 * 12 + 1][k / 4 * 12 + i] = t1;
            B[k % 4 * 12 + 2][k / 4 * 12 + i] = t2;
            B[k % 4 * 12 + 3][k / 4 * 12 + i] = t3;
            B[k % 4 * 12 + 4][k / 4 * 12 + i] = t4;
            B[k % 4 * 12 + 5][k / 4 * 12 + i] = t5;
            B[k % 4 * 12 + 6][k / 4 * 12 + i] = t6;
            B[k % 4 * 12 + 7][k / 4 * 12 + i] = t7;
            B[k % 4 * 12 + 8][k / 4 * 12 + i] = t8;
            B[k % 4 * 12 + 9][k / 4 * 12 + i] = t9;
            B[k % 4 * 12 + 10][k / 4 * 12 + i] = t10;
            B[k % 4 * 12 + 11][k / 4 * 12 + i] = t11;
        }
    }
}
void trans96x96(int M, int N, int A[N][M], int B[M][N]){
    int tmp[12] = {0};
    for(int ai = 0; ai < 96; ai += 12) {
        for(int aj = 0; aj < 96; aj += 12) {
            for(int i = 0; i < 6; i++) {
                for(int j = 0; j < 12; j++)
                    tmp[j] = A[ai + i][aj + j];
                for(int j = 0; j < 6; j++)
                    B[aj + j][ai + i] = tmp[j];
                for(int j = 0; j < 6; j++)
                    B[aj + j][ai + i + 6] = tmp[j + 6];                   
            }
            for(int j = 0; j < 6; j++) {
                for(int i = 0; i < 6; i++)
                    tmp[i] = B[aj + j][ai + i + 6];
                for(int i = 6; i < 12; i++)
                    tmp[i] = A[ai + i][aj + j];
                for(int i = 6; i < 12; i++)
                    B[aj + j][ai + i] = tmp[i];
                for(int i = 0; i < 6; i++)
                    B[aj + j + 6][ai + i] = tmp[i];                                  
            }
            for(int i = 6; i < 12; i++) {
                for(int j = 6; j < 12; j++)
                    tmp[j] = A[ai + i][aj + j];
                for(int j = 6; j < 12; j++)
                    B[aj + j][ai + i] = tmp[j];
            }
        }
    }
}
void trans93x99(int M, int N, int A[N][M], int B[M][N]){
    for(int stepi = 0; stepi < N; stepi += 24){
        for(int stepj = 0; stepj < M; stepj += 24){
            for(int i = 0; i < 24 && (stepi + i) < N; i++){
                for(int j = 0; j < 24 && (stepj + j) < M; j++){
                    B[stepj + j][stepi + i] = A[stepi + i][stepj + j];
                }
            }
        }
    }
}
/*
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started.
 */

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; j++)
        {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    // registerTransFunction(trans, trans_desc);
}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; ++j)
        {
            if (A[i][j] != B[j][i])
            {
                return 0;
            }
        }
    }
    return 1;
}
