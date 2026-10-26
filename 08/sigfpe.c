
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handler(int signum)
{
    const char message[] = "SIGFPE received: arithmetic exception!\n";

    write(STDOUT_FILENO, message, sizeof(message) - 1);
    _exit(EXIT_FAILURE);
}

int main(void)
{
    if (signal(SIGFPE, handler) == SIG_ERR)
    {
        perror("signal");
        return EXIT_FAILURE;
    }

    printf("SIGFPE handler registered.\n");
    printf("Generating an integer division-by-zero error...\n");
    fflush(stdout);

    volatile int numerator = 10;
    volatile int denominator = 0;
    int result = numerator / denominator;

    printf("Result: %d\n", result);

    return 0;
}
