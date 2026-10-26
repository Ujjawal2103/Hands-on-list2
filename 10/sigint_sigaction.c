
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handler(int signum)
{
    const char message[] = "SIGINT caught using sigaction!\n";
    write(STDOUT_FILENO, message, sizeof(message) - 1);
}

int main(void)
{
    struct sigaction sa;

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return EXIT_FAILURE;
    }

    printf("SIGINT handler registered using sigaction.\n");
    printf("Press Ctrl+C to send SIGINT.\n");

    while (1) {
        pause();
    }

    return 0;
}
