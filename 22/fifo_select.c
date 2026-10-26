
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <errno.h>

int main() {
    int fd;
    char buffer[100];

    // Create FIFO if it does not already exist
    if (mkfifo("myfifo", 0666) == -1 && errno != EEXIST) {
        perror("mkfifo");
        return EXIT_FAILURE;
    }

    // Open FIFO in read-write mode (Linux/WSL)
    fd = open("myfifo", O_RDWR | O_NONBLOCK);
    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    fd_set readfds;
    struct timeval timeout;

    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    // Set timeout to 10 seconds
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    printf("Waiting for FIFO data for 10 seconds...\n");
    fflush(stdout);

    int result = select(fd + 1, &readfds, NULL, NULL, &timeout);

    if (result == -1) {
        perror("select");
        close(fd);
        return EXIT_FAILURE;
    } else if (result == 0) {
        printf("Timeout: No data received within 10 seconds.\n");
    } else {
        ssize_t n = read(fd, buffer, sizeof(buffer) - 1);

        if (n > 0) {
            buffer[n] = '\0';
            printf("Data received: %s\n", buffer);
        } else if (n == -1 && errno != EAGAIN) {
            perror("read");
        } else {
            printf("No data available to read.\n");
        }
    }

    close(fd);
    return EXIT_SUCCESS;
}
