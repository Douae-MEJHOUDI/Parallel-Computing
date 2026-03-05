#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 100000000

int main() {
    double a = 1.1, b = 1.2;
    double x = 0.0, y = 0.0;
    clock_t start, end;

    // Manually optimized version:
    // 1. Precompute a*b outside the loop
    // 2. Unroll the loop
    double ab = a * b;

    start = clock();
    for (int i = 0; i < N; i += 2) {
        x += ab;  // iteration i
        y += ab;
        x += ab;  // iteration i+1
        y += ab;
    }
    end = clock();

    printf("x = %f, y = %f, time = %f s\n",
           x, y, (double)(end - start) / CLOCKS_PER_SEC);
    return 0;
}
