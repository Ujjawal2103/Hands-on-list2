
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {
    key_t key;
    int msgid;
    struct msqid_ds info;

    // Generate a key
    key = ftok(".", 'B');

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

    // Retrieve queue metadata
    if (msgctl(msgid, IPC_STAT, &info) == -1) {
        perror("msgctl");
        return EXIT_FAILURE;
    }

    printf("Message Queue ID: %d\n", msgid);
    printf("Queue Key: %d\n", (int)key);

    printf("\n--- Permissions and Ownership ---\n");
    printf("Owner UID: %u\n", (unsigned int)info.msg_perm.uid);
    printf("Owner GID: %u\n", (unsigned int)info.msg_perm.gid);
    printf("Creator UID: %u\n", (unsigned int)info.msg_perm.cuid);
    printf("Creator GID: %u\n", (unsigned int)info.msg_perm.cgid);
    printf("Permissions: %o\n",
           (unsigned int)info.msg_perm.mode);

    printf("\n--- Queue Information ---\n");
    printf("Number of messages: %lu\n",
           (unsigned long)info.msg_qnum);
    printf("Maximum queue bytes: %lu\n",
           (unsigned long)info.msg_qbytes);
    printf("Last sender PID: %d\n", (int)info.msg_lspid);
    printf("Last receiver PID: %d\n", (int)info.msg_lrpid);

    return EXIT_SUCCESS;
}
