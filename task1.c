#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    char buffer[100];
    int fd = open("example.txt", O_RDONLY);
    if (fd < 0) {
        perror("Failed to open file");
        return 1;
    }
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);
    if (bytesRead < 0) {
        perror("Failed to read file");
        return 1;
    }
    buffer[bytesRead] = '\0';
    printf("Data read: %s\n", buffer);
    close(fd);
    return 0;
}
//sysdeps/unix/sysv/linux/read.c:/* Linux read syscall implementation.

//./mm/readahead.c:SYSCALL_DEFINE3(readahead, int, fd, loff_t, offset, size_t, count)