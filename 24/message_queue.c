
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {
    key_t key;
    int msgid;

    // Generate a key using the current directory
    key = ftok(".", 'A');

    if (key == -1) {
        perror("ftok");
        return EXIT_FAILURE;
    }

    // Create or access the message queue
    msgid = msgget(key, IPC_CREAT | 0666);

    if (msgid == -1) {
        perror("msgget");
        return EXIT_FAILURE;
    }

    printf("Message Queue Key: %d\n", (int)key);
    printf("Message Queue ID: %d\n", msgid);

    return EXIT_SUCCESS;
}
