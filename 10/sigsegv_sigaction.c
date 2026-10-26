
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handler(int signum)
{
    const char message[] = "SIGSEGV caught using sigaction!\n";

    write(STDOUT_FILENO, message, sizeof(message) - 1);
    _exit(EXIT_FAILURE);
}

int main(void)
{
    struct sigaction sa;

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGSEGV, &sa, NULL) == -1)
    {
        perror("sigaction");
        return EXIT_FAILURE;
    }

    printf("SIGSEGV handler registered using sigaction.\n");
    printf("Waiting for SIGSEGV...\n");
    fflush(stdout);

    while (1)
    {
        pause();
    }

    return 0;
}
