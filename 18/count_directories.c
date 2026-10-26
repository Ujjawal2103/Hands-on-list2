
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int p1[2], p2[2];

    if (pipe(p1) == -1 || pipe(p2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t ls_pid = fork();

    if (ls_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (ls_pid == 0) {
        // First process: ls -l
        dup2(p1[1], STDOUT_FILENO);

        close(p1[0]);
        close(p1[1]);
        close(p2[0]);
        close(p2[1]);

        execlp("ls", "ls", "-l", (char *)NULL);
        perror("execlp ls");
        exit(EXIT_FAILURE);
    }

    pid_t grep_pid = fork();

    if (grep_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (grep_pid == 0) {
        // Second process: grep ^d
        dup2(p1[0], STDIN_FILENO);
        dup2(p2[1], STDOUT_FILENO);

        close(p1[0]);
        close(p1[1]);
        close(p2[0]);
        close(p2[1]);

        execlp("grep", "grep", "^d", (char *)NULL);
        perror("execlp grep");
        exit(EXIT_FAILURE);
    }

    pid_t wc_pid = fork();

    if (wc_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (wc_pid == 0) {
        // Third process: wc -l
        dup2(p2[0], STDIN_FILENO);

        close(p1[0]);
        close(p1[1]);
        close(p2[0]);
        close(p2[1]);

        execlp("wc", "wc", "-l", (char *)NULL);
        perror("execlp wc");
        exit(EXIT_FAILURE);
    }

    // Parent closes all pipe descriptors
    close(p1[0]);
    close(p1[1]);
    close(p2[0]);
    close(p2[1]);

    // Wait for all three child processes
    waitpid(ls_pid, NULL, 0);
    waitpid(grep_pid, NULL, 0);
    waitpid(wc_pid, NULL, 0);

    return 0;
}
