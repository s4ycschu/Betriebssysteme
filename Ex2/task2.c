#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ITERATIONS 1000000

sem_t semaphore;
volatile int shared_data = 0;

void *thread_func(void *arg) {
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        sem_wait(&semaphore);
        shared_data++;
        sem_post(&semaphore);
    }
    return NULL;
}

int main() {
    pthread_t thread;
    struct timespec start, end;
    double elapsed_time;

    // Initialize semaphore
    sem_init(&semaphore, 0, 1);

    // Start timing
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Create a second thread
    pthread_create(&thread, NULL, thread_func, NULL);

    for (int i = 0; i < NUM_ITERATIONS; i++) {
        sem_wait(&semaphore);
        shared_data++;
        sem_post(&semaphore);
    }

    pthread_join(thread, NULL);

    // End timing
    clock_gettime(CLOCK_MONOTONIC, &end);

    elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Elapsed time for Semaphore (2 threads): %f seconds\n", elapsed_time);

    sem_destroy(&semaphore);
    return 0;
}
