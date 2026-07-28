#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

extern void asmhello();

void distance_kernel(const float* X1, const float* X2, const float* Y1, const float* Y2, float* Z, int n);

int main(void)
{
    asmhello();

    //test values
    float X1[] = {1.5f, 4.0f, 3.5f, 2.0f};
    float X2[] = {3.0f, 2.5f, 2.5f, 1.0f};
    float Y1[] = {4.0f, 3.0f, 3.5f, 3.0f};
    float Y2[] = {2.0f, 2.5f, 1.0f, 1.5f};

    float Z[4];

    //call kernel function
    distance_kernel(X1, X2, Y1, Y2, Z, 4);

    //display results
    for (int i = 0; i < 4; i++) {
        printf("Z[%d] = %f\n", i, Z[i]);
    }

    return 0;
}

//kernel formula
void distance_kernel(const float* X1, const float* X2, const float* Y1, const float* Y2, float* Z, int n)
{
    for (int i = 0; i < n; i++) {
        float dx = X2[i] - X1[i];
        float dy = Y2[i] - Y1[i];

        Z[i] = sqrtf((dx * dx) + (dy * dy));
    }
}
