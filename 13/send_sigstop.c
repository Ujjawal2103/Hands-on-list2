
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s <PID>\n", argv[0]);
        return EXIT_FAILURE;
    }

    pid_t pid = (pid_t)atoi(argv[1]);

    if (kill(pid, SIGSTOP) == -1) {
        perror("kill");
        return EXIT_FAILURE;
    }

    printf("SIGSTOP sent to process %d\n", (int)pid);

    return EXIT_SUCCESS;
}
