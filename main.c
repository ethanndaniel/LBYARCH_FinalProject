#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

extern void asmhello();

void distance_kernel(const float* X1, const float* X2, const float* Y1, const float* Y2, float* Z, int n);
void generate_values(float* X1, float* X2, float* Y1, float* Y2, int n);
void load_sample_values(float* X1, float* X2, float* Y1, float* Y2);
double benchmark_kernel(const float* X1, const float* X2, const float* Y1, const float* Y2, float* Z, int n);

int main(void) {
    asmhello();

    int n = 4;
    //initialize memory allocation
    float* X1 = (float*)malloc(n * sizeof(float));
    float* X2 = (float*)malloc(n * sizeof(float));
    float* Y1 = (float*)malloc(n * sizeof(float));
    float* Y2 = (float*)malloc(n * sizeof(float));
    float* Z =  (float*)malloc(n * sizeof(float));

    //verify allocation
    if (X1 == NULL || X2 == NULL || Y1 == NULL || Y2 == NULL || Z == NULL) {
        
        printf("Memory allocation failed.\n");

        free(X1);
        free(X2);
        free(Y1);
        free(Y2);
        free(Z);

        return 1;
    }

    //compute for execution time using sample values
    load_sample_values(X1, X2, Y1, Y2);
    double finalTime = benchmark_kernel(X1, X2, Y1, Y2, Z, n);

    //display results
    for (int i = 0; i < n; i++) {
        printf("Z[%d] = %.9f\n", i, Z[i]);
    }

    printf("\nAverage Execution Time (30 runs): %.9f seconds\n", finalTime);

    //free the memory
    free(X1);
    free(X2);
    free(Y1);
    free(Y2);
    free(Z);

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
double benchmark_kernel(const float* X1, const float* X2, const float* Y1, const float* Y2, float* Z, int n) {
    
    LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    LARGE_INTEGER end;

    QueryPerformanceFrequency(&frequency);

    double finalTime = 0.0;

    for (int i = 0; i < 30; i++)
    {
        QueryPerformanceCounter(&start);

        distance_kernel(X1, X2, Y1, Y2, Z, n);

        QueryPerformanceCounter(&end);

        finalTime += (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
    }

    return finalTime / 30.0;
}
