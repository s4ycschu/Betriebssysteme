#include <zmq.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_ITERATIONS 1000000
#define MESSAGE_SIZE 10

int main() {
    void *context = zmq_ctx_new();
    void *sender = zmq_socket(context, ZMQ_PAIR);
    void *receiver = zmq_socket(context, ZMQ_PAIR);

    zmq_bind(sender, "inproc://test");
    zmq_connect(receiver, "inproc://test");

    struct timespec start, end;
    double elapsed_time;
    char message[MESSAGE_SIZE] = "ping";

    // Start timing
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < NUM_ITERATIONS; i++) {
        zmq_send(sender, message, MESSAGE_SIZE, 0);
        zmq_recv(receiver, message, MESSAGE_SIZE, 0);
    }

    // End timing
    clock_gettime(CLOCK_MONOTONIC, &end);

    elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Elapsed time for ZeroMQ (inproc): %f seconds\n", elapsed_time);

    zmq_close(sender);
    zmq_close(receiver);
    zmq_ctx_destroy(context);
    return 0;
}
