
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        /* Child process */
        pid_t parent_pid = getppid();

        printf("Child process created.\n");
        printf("Child PID: %d\n", getpid());
        printf("Original parent PID: %d\n", parent_pid);
        fflush(stdout);

        sleep(1);

        printf("Child is sending SIGKILL to its parent.\n");
        fflush(stdout);

        if (kill(parent_pid, SIGKILL) == -1) {
            perror("kill");
            return EXIT_FAILURE;
        }

        sleep(1);

        printf("New parent PID: %d\n", getppid());
        printf("Child process is exiting.\n");

        return EXIT_SUCCESS;
    } else {
        /* Parent process */
        printf("Parent process created.\n");
        printf("Parent PID: %d\n", getpid());
        printf("Child PID: %d\n", pid);
        fflush(stdout);

        while (1) {
            pause();
        }
    }

    return EXIT_SUCCESS;
}
