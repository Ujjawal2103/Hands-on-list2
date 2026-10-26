
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {
    key_t key;
    int msgid;

    // Generate the same key used in Program 26
    key = ftok("../26", 'B');

    if (key == -1) {
        perror("ftok");
        return EXIT_FAILURE;
    }

    // Access the existing queue
    msgid = msgget(key, 0666);

    if (msgid == -1) {
        perror("msgget");
        printf("Message queue does not exist.\n");
        return EXIT_FAILURE;
    }

    printf("Message Queue ID: %d\n", msgid);

    // Remove the queue
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl IPC_RMID");
        return EXIT_FAILURE;
    }

    printf("Message queue removed successfully.\n");

    return EXIT_SUCCESS;
}
