
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

struct message {
    long mtype;
    char mtext[100];
};

int main(int argc, char *argv[]) {
    key_t key;
    int msgid, flags;
    struct message msg;
    ssize_t bytes;

    if (argc != 2 ||
        (strcmp(argv[1], "0") != 0 &&
         strcmp(argv[1], "nowait") != 0)) {
        printf("Usage: %s 0|nowait\n", argv[0]);
        return EXIT_FAILURE;
    }

    key = ftok("../26", 'B');
    if (key == -1) {
        perror("ftok");
        return EXIT_FAILURE;
    }

    msgid = msgget(key, 0666);
    if (msgid == -1) {
        perror("msgget");
        printf("Run Program 26 first to create the queue and send a message.\n");
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "nowait") == 0)
        flags = IPC_NOWAIT;
    else
        flags = 0;

    // Receive the first available message, regardless of its type
    bytes = msgrcv(msgid, &msg, sizeof(msg.mtext), 0, flags);

    if (bytes == -1) {
        perror("msgrcv");
        return EXIT_FAILURE;
    }

    printf("Message received successfully!\n");
    printf("Message type: %ld\n", msg.mtype);
    printf("Message: %s\n", msg.mtext);

    return EXIT_SUCCESS;
}
