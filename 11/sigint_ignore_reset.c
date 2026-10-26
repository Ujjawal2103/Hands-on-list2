
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(void)
{
    struct sigaction sa;

    /* Ignore SIGINT */
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return EXIT_FAILURE;
    }

    printf("SIGINT is ignored for 10 seconds. Press Ctrl+C.\n");
    fflush(stdout);
    sleep(10);

    /* Restore the default action */
    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return EXIT_FAILURE;
    }

    printf("Default SIGINT action restored. Press Ctrl+C now.\n");
    fflush(stdout);

    while (1) {
        pause();
    }

    return 0;
}
