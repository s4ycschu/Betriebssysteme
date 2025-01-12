#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>
#include <stdint.h>

#define ITERATIONS 1000000
#define PORT 5555
#define BUFFER_SIZE 1

uint64_t get_time_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000LL + ts.tv_nsec;
}

void* server_worker(void* arg) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 1);
    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

    for (int i = 0; i < ITERATIONS; ++i) {
        read(new_socket, buffer, BUFFER_SIZE);
        write(new_socket, buffer, BUFFER_SIZE);
    }

    close(new_socket);
    close(server_fd);
    return NULL;
}

int main() {
    pthread_t server_thread;
    pthread_create(&server_thread, NULL, server_worker, NULL);
    sleep(1);  // Warten, bis der Server bereit ist

    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {'A'};

    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    uint64_t start, end, total = 0;
    for (int i = 0; i < ITERATIONS; ++i) {
        start = get_time_ns();
        write(sock, buffer, BUFFER_SIZE);
        read(sock, buffer, BUFFER_SIZE);
        end = get_time_ns();
        total += (end - start);
    }

    printf("Average Docker TCP Latency: %.2f ns\n", (double)total / ITERATIONS);
    close(sock);
    pthread_join(server_thread, NULL);
    return 0;
}
