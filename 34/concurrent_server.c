
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PORT 8080

void *handle_client(void *arg) {
    int client_fd = (int)(intptr_t)arg;
    char buffer[1024];
    const char *reply = "Hello from concurrent server!";

    ssize_t bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

    if (bytes > 0) {
        buffer[bytes] = '\0';
        printf("Client message: %s\n", buffer);

        if (send(client_fd, reply, strlen(reply), 0) == -1)
            perror("send");
    } else if (bytes == -1) {
        perror("recv");
    }

    close(client_fd);
    return NULL;
}

int main() {
    int server_fd;
    struct sockaddr_in address;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
                   &opt, sizeof(opt)) == -1) {
        perror("setsockopt");
        close(server_fd);
        return EXIT_FAILURE;
    }

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) == -1) {
        perror("bind");
        close(server_fd);
        return EXIT_FAILURE;
    }

    if (listen(server_fd, 10) == -1) {
        perror("listen");
        close(server_fd);
        return EXIT_FAILURE;
    }

    // Let the system reap terminated child processes
    signal(SIGCHLD, SIG_IGN);

    printf("Concurrent server listening on port %d...\n", PORT);

    while (1) {
        int client_fd = accept(server_fd, NULL, NULL);

        if (client_fd == -1) {
            if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
                perror("signal");
                break;
            }
            perror("accept");
            continue;
        }

        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            close(client_fd);
            continue;
        }

        if (pid == 0) {
            // Child process handles this client using a thread
            close(server_fd);

            pthread_t tid;
            int err = pthread_create(&tid, NULL,
                                     handle_client,
                                     (void *)(intptr_t)client_fd);

            if (err != 0) {
                fprintf(stderr, "pthread_create: %s\n",
                        strerror(err));
                close(client_fd);
                _exit(EXIT_FAILURE);
            }

            pthread_join(tid, NULL);
            _exit(EXIT_SUCCESS);
        }

        // Parent continues accepting new clients
        close(client_fd);
    }

    close(server_fd);
    return EXIT_SUCCESS;
}
