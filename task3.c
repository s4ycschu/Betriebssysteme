#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define NUM_SWITCHES 10000

int pipefd[2];  // Pipe für Kommunikation

void* thread_func(void* arg) {
    char buf = 'x';
    for (int i = 0; i < NUM_SWITCHES; i++) {
        // Warten, bis Hauptthread ein Signal gibt
        read(pipefd[0], &buf, 1);
        // Signal zurück an Hauptthread senden
        write(pipefd[1], &buf, 1);
    }
    return NULL;
}

int main() {
    pthread_t thread;
    struct timeval start, end;
    char buf = 'x';

    // Pipe erstellen
    pipe(pipefd);

    // Zweiten Thread starten
    pthread_create(&thread, NULL, thread_func, NULL);

    // Startzeit erfassen
    gettimeofday(&start, NULL);

    for (int i = 0; i < NUM_SWITCHES; i++) {
        // Signal an den anderen Thread senden
        write(pipefd[1], &buf, 1);
        // Warten, bis der andere Thread antwortet
        read(pipefd[0], &buf, 1);
    }

    // Endzeit erfassen
    gettimeofday(&end, NULL);

    // Thread beenden
    pthread_join(thread, NULL);

    // Zeit berechnen
    double total_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    double avg_switch_time = total_time / (NUM_SWITCHES * 2);  // Zwei Kontextwechsel pro Iteration

    printf("Total time: %f seconds\n", total_time);
    printf("Average context switch time: %f microseconds\n", avg_switch_time * 1e6);

    return 0;
}
//Total time: 0.015714 seconds
//Average context switch time: 0.785700 microseconds