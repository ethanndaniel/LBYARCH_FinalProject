#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <time.h>

extern void asmkernel(const float* X1, const float* X2, const float* Y1, const float* Y2, float* Z, int n);

void distance_kernel(const float* X1, const float* X2, const float* Y1, const float* Y2, float* Z, int n);
void generate_values(float* X1, float* X2, float* Y1, float* Y2, int n);
void load_sample_values(float* X1, float* X2, float* Y1, float* Y2);
typedef void (*kernel_func)(const float*, const float*, const float*, const float*, float*, int);
double benchmark_kernel(kernel_func kernel, const float* X1, const float* X2, const float* Y1, const float* Y2, float* Z, int n);

int main(void) {
    
	//for random number generation
    srand((unsigned)time(NULL));

    int n = 4;

	//n = 2^20, 2^24, 2^30 for testing
    //n = 2^30 laptop couldnt handle it so reduced it to 27 instead
    int testSizes[] = {1 << 20,1 << 24,1 << 27};
    int numTests = 3;

    for (int t = 0; t < numTests; t++) {
        int n = testSizes[t];


        //initialize memory allocation
        float* X1 = (float*)malloc(n * sizeof(float));
        float* X2 = (float*)malloc(n * sizeof(float));
        float* Y1 = (float*)malloc(n * sizeof(float));
        float* Y2 = (float*)malloc(n * sizeof(float));
        float* Z = (float*)malloc(n * sizeof(float));
        float* Z_asm = (float*)malloc(n * sizeof(float));

        //verify allocation
        if (X1 == NULL || X2 == NULL || Y1 == NULL || Y2 == NULL || Z == NULL || Z_asm == NULL) {

            printf("Memory allocation failed.\n");

            free(X1);
            free(X2);
            free(Y1);
            free(Y2);
            free(Z);
            free(Z_asm);

            return 1;
        }

        //compute for execution time using sample values

        printf("\nTesting n = %d\n", n);
        printf("Generating values:\n");

        generate_values(X1, X2, Y1, Y2, n);

        printf("Benchmarking C:\n");
        double finalTime_C = benchmark_kernel(distance_kernel, X1, X2, Y1, Y2, Z, n);
        printf("Benchmarking ASM:\n");
        double finalTime_asm = benchmark_kernel(asmkernel, X1, X2, Y1, Y2, Z_asm, n);
        //display first 10 results for C
        int limit = (n < 10) ? n : 10;

        for (int i = 0; i < limit; i++) {
            printf("Z[%d] = %.9f\n", i, Z[i]);
        }
        printf("\nAverage Execution Time for C (30 runs): %.9f seconds\n", finalTime_C);

        //display first 10 results for ASM
        for (int i = 0; i < limit; i++) {
            printf("Z_asm[%d] = %.9f\n", i, Z[i]);
        }
        printf("\nAverage Execution Time for ASM (30 runs): %.9f seconds\n", finalTime_asm);

        //verify correctness of asm kernel
        int correct = 1;

        for (int i = 0; i < n; i++)
        {
            //absolute value of C kernels minus ASM kernel
            if (fabsf(Z[i] - Z_asm[i]) > 0.000001)
            {
                correct = 0;
                printf("Not equal at index %d\n", i);
                break;
            }
        }
        if (correct)
            printf("asm kernel is correct.\n");
        else
            printf("asm kernel is incorrect.\n");

        //free the memory
        free(X1);
        free(X2);
        free(Y1);
        free(Y2);
        free(Z);
        free(Z_asm);
    }
    return 0;
}

//kernel formula
void distance_kernel(const float* X1, const float* X2, const float* Y1, const float* Y2, float* Z, int n) {
    for (int i = 0; i < n; i++) {
        float dx = X2[i] - X1[i];
        float dy = Y2[i] - Y1[i];

        Z[i] = sqrtf((dx * dx) + (dy * dy));
    }
}

//sample values for testing
void load_sample_values(float* X1, float* X2, float* Y1, float* Y2) {
    X1[0] = 1.5f;
    X1[1] = 4.0f;
    X1[2] = 3.5f;
    X1[3] = 2.0f;

    X2[0] = 3.0f;
    X2[1] = 2.5f;
    X2[2] = 2.5f;
    X2[3] = 1.0f;

    Y1[0] = 4.0f;
    Y1[1] = 3.0f;
    Y1[2] = 3.5f;
    Y1[3] = 3.0f;

    Y2[0] = 2.0f;
    Y2[1] = 2.5f;
    Y2[2] = 1.0f;
    Y2[3] = 1.5f;
}

//generate random values
void generate_values(float* X1, float* X2, float* Y1, float* Y2, int n) {
    for (int i = 0; i < n; i++) {
        X1[i] = (float)(rand() % 100);
        X2[i] = (float)(rand() % 100);
        Y1[i] = (float)(rand() % 100);
        Y2[i] = (float)(rand() % 100);
    }
}

//for getting average time with 30 runs
double benchmark_kernel(kernel_func kernel, const float* X1, const float* X2, const float* Y1, const float* Y2, float* Z, int n) {

    LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    LARGE_INTEGER end;

    QueryPerformanceFrequency(&frequency);

    double finalTime = 0.0;

    for (int i = 0; i < 30; i++)
    {
        QueryPerformanceCounter(&start);

        kernel(X1, X2, Y1, Y2, Z, n);

        QueryPerformanceCounter(&end);

        finalTime += (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
    }

    return finalTime / 30.0;
}
