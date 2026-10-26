
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int fd[2];
    char write_msg[] = "Hello from parent process!";
    char read_msg[100];

    // Create the pipe
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Create child process
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // Parent process
        close(fd[0]);  // Parent only writes

        write(fd[1], write_msg, strlen(write_msg) + 1);
        close(fd[1]);

        wait(NULL);
    } else {
        // Child process
        close(fd[1]);  // Child only reads

        read(fd[0], read_msg, sizeof(read_msg));
        printf("Child received: %s\n", read_msg);

        close(fd[0]);
        exit(EXIT_SUCCESS);
    }

    return 0;
}
