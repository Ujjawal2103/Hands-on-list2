
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    int fd[2];

    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child: ls -l writes into the pipe
        close(fd[0]);
        close(STDOUT_FILENO);

        if (fcntl(fd[1], F_DUPFD, STDOUT_FILENO) == -1) {
            perror("fcntl");
            exit(EXIT_FAILURE);
        }

        close(fd[1]);

        execlp("ls", "ls", "-l", (char *)NULL);
        perror("execlp ls");
        exit(EXIT_FAILURE);
    } else {
        // Parent: wc reads from the pipe
        close(fd[1]);
        close(STDIN_FILENO);

        if (fcntl(fd[0], F_DUPFD, STDIN_FILENO) == -1) {
            perror("fcntl");
            exit(EXIT_FAILURE);
        }

        close(fd[0]);

        execlp("wc", "wc", (char *)NULL);
        perror("execlp wc");
        exit(EXIT_FAILURE);
    }

    return 0;
}
