
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <fcntl.h>

int main() {
    struct rlimit limit;
    int fd[2];

    // Get maximum open files per process
    if (getrlimit(RLIMIT_NOFILE, &limit) == -1) {
        perror("getrlimit");
        return EXIT_FAILURE;
    }

    printf("Maximum open files (soft limit): ");

    if (limit.rlim_cur == RLIM_INFINITY)
        printf("Unlimited\n");
    else
        printf("%llu\n", (unsigned long long)limit.rlim_cur);

    printf("Maximum open files (hard limit): ");

    if (limit.rlim_max == RLIM_INFINITY)
        printf("Unlimited\n");
    else
        printf("%llu\n", (unsigned long long)limit.rlim_max);

    // Create a pipe
    if (pipe(fd) == -1) {
        perror("pipe");
        return EXIT_FAILURE;
    }

    // Get pipe buffer capacity
    int pipe_size = fcntl(fd[0], F_GETPIPE_SZ);

    if (pipe_size == -1) {
        perror("fcntl F_GETPIPE_SZ");
        close(fd[0]);
        close(fd[1]);
        return EXIT_FAILURE;
    }

    printf("Pipe buffer size: %d bytes\n", pipe_size);

    close(fd[0]);
    close(fd[1]);

    return EXIT_SUCCESS;
}
