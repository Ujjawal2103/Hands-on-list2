
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd;
    char message[] = "Hello from the writer program!";

    fd = open("myfifo", O_WRONLY);
    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    if (write(fd, message, strlen(message) + 1) == -1) {
        perror("write");
        close(fd);
        return EXIT_FAILURE;
    }

    printf("Message sent: %s\n", message);

    close(fd);
    return EXIT_SUCCESS;
}

