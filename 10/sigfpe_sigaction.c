
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handler(int signum)
{
    const char message[] = "SIGFPE caught using sigaction!\n";
    write(STDOUT_FILENO, message, sizeof(message) - 1);
    _exit(EXIT_SUCCESS);
}

int main(void)
{
    struct sigaction sa;

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGFPE, &sa, NULL) == -1) {
        perror("sigaction");
        return EXIT_FAILURE;
    }

    printf("SIGFPE handler registered using sigaction.\n");
    printf("Generating SIGFPE...\n");
    fflush(stdout);

    raise(SIGFPE);

    printf("This line will not execute.\n");

    return 0;
}
