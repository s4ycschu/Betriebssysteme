#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sched.h>

#define NUM_ITERATIONS 1000000

// Definition atomic_flag
typedef struct {
    atomic_flag flag;
} spinlock_t;

// Spinlock wird initialisiert
void spinlock_init(spinlock_t *lock) {
    atomic_flag_clear(&lock->flag);
}

// adaptive Spinning
void spinlock_lock(spinlock_t *lock) {
    int tries = 0;
    while (atomic_flag_test_and_set_explicit(&lock->flag, memory_order_acquire)) {
        if (tries++ > 1000) {
            sched_yield();  // CPU-Zeit freigeben, wenn oft versucht
            tries = 0;
        } else {
            __asm__ __volatile__("yield");  // ARM-Optimierung: effizienteres Warten
        }
    }
}

// Spinlock freigeben
void spinlock_unlock(spinlock_t *lock) {
    atomic_flag_clear_explicit(&lock->flag, memory_order_release);
}

volatile int shared_data = 0;
spinlock_t spinlock;

// Thread-Funktion für inkrementelle Operation
void *thread_func(void *arg) {
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        spinlock_lock(&spinlock);
        shared_data++;
        spinlock_unlock(&spinlock);
    }
    return NULL;
}

int main() {
    pthread_t thread;
    struct timespec start, end;
    double elapsed_time;

    // Spinlock initialisieren
    spinlock_init(&spinlock);

    // Startzeit messen
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Zweiten Thread starten
    pthread_create(&thread, NULL, thread_func, NULL);

    // Hauptthread führt ebenfalls Operationen durch
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        spinlock_lock(&spinlock);
        shared_data++;
        spinlock_unlock(&spinlock);
    }

    pthread_join(thread, NULL);

    // Endzeit messen
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Berechnung der verstrichenen Zeit
    elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Elapsed time for optimized Spinlock (2 threads): %f seconds\n", elapsed_time);
    printf("Final shared_data value: %d\n", shared_data);

    return 0;
}
