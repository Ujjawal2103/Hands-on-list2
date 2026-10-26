
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd[2];
    char write_msg[] = "Hello through pipe!";
    char read_msg[100];

    // Create the pipe
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Write data into the pipe
    write(fd[1], write_msg, strlen(write_msg) + 1);

    // Read data from the pipe
    read(fd[0], read_msg, sizeof(read_msg));

    // Display the received data
    printf("Message received: %s\n", read_msg);

    // Close both ends of the pipe
    close(fd[0]);
    close(fd[1]);

    return 0;
}
