#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct message {
    long mtype;
    char mtext[100];
};

int main() {
    key_t key;
    int msgid;
    struct message msg;

    key = ftok(".", 'B');

    if (key == -1) {
        perror("ftok");
        return EXIT_FAILURE;
    }

    msgid = msgget(key, IPC_CREAT | 0666);

    if (msgid == -1) {
        perror("msgget");
        return EXIT_FAILURE;
    }

    msg.mtype = 1;
    strcpy(msg.mtext, "Hello from message queue!");

    if (msgsnd(msgid, &msg, strlen(msg.mtext) + 1, 0) == -1) {
        perror("msgsnd");
        return EXIT_FAILURE;
    }

    printf("Message sent successfully!\n");
    printf("Message Queue ID: %d\n", msgid);
    printf("Message: %s\n", msg.mtext);

    return EXIT_SUCCESS;
}
