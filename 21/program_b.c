
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int read_fd, write_fd;
    char buffer[100];
    char reply[] = "Hello from Program B!";

    // Open FIFO for receiving from A
    read_fd = open("fifo_ab", O_RDONLY);
    if (read_fd == -1) {
        perror("open fifo_ab");
        return EXIT_FAILURE;
    }

    // Open FIFO for sending to A
    write_fd = open("fifo_ba", O_WRONLY);
    if (write_fd == -1) {
        perror("open fifo_ba");
        close(read_fd);
        return EXIT_FAILURE;
    }

    // Receive message from A
    ssize_t n = read(read_fd, buffer, sizeof(buffer) - 1);
    if (n == -1) {
        perror("read");
        return EXIT_FAILURE;
    }

    buffer[n] = '\0';
    printf("Program B received: %s\n", buffer);

    // Send reply to A
    if (write(write_fd, reply, strlen(reply) + 1) == -1) {
        perror("write");
        return EXIT_FAILURE;
    }
    printf("Program B sent: %s\n", reply);

    close(read_fd);
    close(write_fd);

    return EXIT_SUCCESS;
}
