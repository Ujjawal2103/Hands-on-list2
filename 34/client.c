
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in server_address;
    char buffer[1024] = {0};
    const char *server_ip = "127.0.0.1";
    const char *message = "Hello from client!";

    if (argc >= 2)
        server_ip = argv[1];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    if (inet_pton(AF_INET, server_ip,
                  &server_address.sin_addr) != 1) {
        fprintf(stderr, "Invalid server IP address\n");
        close(sock);
        return EXIT_FAILURE;
    }

    if (connect(sock, (struct sockaddr *)&server_address,
                sizeof(server_address)) == -1) {
        perror("connect");
        close(sock);
        return EXIT_FAILURE;
    }

    if (send(sock, message, strlen(message), 0) == -1) {
        perror("send");
        close(sock);
        return EXIT_FAILURE;
    }

    printf("Message sent to server.\n");

    ssize_t bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);

    if (bytes == -1) {
        perror("recv");
    } else if (bytes == 0) {
        printf("Server closed the connection.\n");
    } else {
        buffer[bytes] = '\0';
        printf("Server reply: %s\n", buffer);
    }

    close(sock);
    return EXIT_SUCCESS;
}
