
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int p1[2], p2[2];
    char parent_msg[] = "Hello from parent!";
    char child_msg[] = "Hello from child!";
    char buffer[100];

    // Create two pipes
    if (pipe(p1) == -1 || pipe(p2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // Parent process
        close(p1[0]);  // Parent writes to p1
        close(p2[1]);  // Parent reads from p2

        // Send message to child
        write(p1[1], parent_msg, strlen(parent_msg) + 1);
        close(p1[1]);

        // Receive reply from child
        read(p2[0], buffer, sizeof(buffer));
        printf("Parent received: %s\n", buffer);

        close(p2[0]);
        wait(NULL);

    } else {
        // Child process
        close(p1[1]);  // Child reads from p1
        close(p2[0]);  // Child writes to p2

        // Receive message from parent
        read(p1[0], buffer, sizeof(buffer));
        printf("Child received: %s\n", buffer);
        close(p1[0]);

        // Send reply to parent
        write(p2[1], child_msg, strlen(child_msg) + 1);
        close(p2[1]);

        exit(EXIT_SUCCESS);
    }

    return 0;
}
