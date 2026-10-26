
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int write_fd, read_fd;
    char message[] = "Hello from Program A!";
    char buffer[100];

    // Open FIFO for sending to B
    write_fd = open("fifo_ab", O_WRONLY);
    if (write_fd == -1) {
        perror("open fifo_ab");
        return EXIT_FAILURE;
    }

    // Open FIFO for receiving from B
    read_fd = open("fifo_ba", O_RDONLY);
    if (read_fd == -1) {
        perror("open fifo_ba");
        close(write_fd);
        return EXIT_FAILURE;
    }

    // Send message to B
    if (write(write_fd, message, strlen(message) + 1) == -1) {
        perror("write");
        return EXIT_FAILURE;
    }
    printf("Program A sent: %s\n", message);

    // Receive reply from B
    ssize_t n = read(read_fd, buffer, sizeof(buffer) - 1);
    if (n == -1) {
        perror("read");
        return EXIT_FAILURE;
    }

    buffer[n] = '\0';
    printf("Program A received: %s\n", buffer);

    close(write_fd);
    close(read_fd);

    return EXIT_SUCCESS;
}

