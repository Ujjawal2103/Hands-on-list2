
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    if (mknod("fifo_system_call", S_IFIFO | 0666, 0) == -1) {
        perror("mknod");
        return EXIT_FAILURE;
    }

    printf("FIFO created using mknod() system call.\n");
    return EXIT_SUCCESS;
}
