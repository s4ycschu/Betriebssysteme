#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define NUM_ITERATIONS 10000000

int main() {
    struct timespec start, end;
    double elapsed_time;
    char buffer = 'a';

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < NUM_ITERATIONS; i++) {
        write(STDOUT_FILENO, &buffer, 1);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    double latency_per_call = elapsed_time / NUM_ITERATIONS;

    printf("Total time for %d iterations: %f seconds\n", NUM_ITERATIONS, elapsed_time);
    printf("Average latency per system call: %f microseconds\n", latency_per_call * 1e6);

    return 0;
}
//Total time for 10000000 iterations: 0.019183 seconds
//Average latency per system call: 0.001918 microseconds
// auf dem Mac M1 Pro