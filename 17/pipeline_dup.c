
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
        // Child: execute ls -l
        close(fd[0]);
        close(STDOUT_FILENO);
        dup(fd[1]);

        close(fd[1]);

        execlp("ls", "ls", "-l", (char *)NULL);
        perror("execlp ls");
        exit(EXIT_FAILURE);
    } else {
        // Parent: execute wc
        close(fd[1]);
        close(STDIN_FILENO);
        dup(fd[0]);

        close(fd[0]);

        wait(NULL);
        execlp("wc", "wc", (char *)NULL);
        perror("execlp wc");
        exit(EXIT_FAILURE);
    }

    return 0;
}
