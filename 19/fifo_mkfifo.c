
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    if (mkfifo("fifo_library", 0666) == -1) {
        perror("mkfifo");
        return EXIT_FAILURE;
    }

    printf("FIFO created using mkfifo() library function.\n");
    return EXIT_SUCCESS;
}
