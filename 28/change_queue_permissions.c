
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {
    key_t key;
    int msgid;
    struct msqid_ds info;

    // Use the same key as Program 26
    key = ftok("../26", 'B');

    if (key == -1) {
        perror("ftok");
        return EXIT_FAILURE;
    }

    // Access the existing message queue
    msgid = msgget(key, 0666);

    if (msgid == -1) {
        perror("msgget");
        printf("Run Program 26 first to create the message queue.\n");
        return EXIT_FAILURE;
    }

    // Get current queue metadata
    if (msgctl(msgid, IPC_STAT, &info) == -1) {
        perror("msgctl IPC_STAT");
        return EXIT_FAILURE;
    }

    printf("Queue ID: %d\n", msgid);
    printf("Permissions before change: %o\n",
           (unsigned int)(info.msg_perm.mode & 0777));

    // Change permissions to owner read/write only
    info.msg_perm.mode =
        (info.msg_perm.mode & ~0777) | 0600;

    if (msgctl(msgid, IPC_SET, &info) == -1) {
        perror("msgctl IPC_SET");
        return EXIT_FAILURE;
    }

    // Read the updated permissions
    if (msgctl(msgid, IPC_STAT, &info) == -1) {
        perror("msgctl IPC_STAT");
        return EXIT_FAILURE;
    }

    printf("Permissions after change: %o\n",
           (unsigned int)(info.msg_perm.mode & 0777));

    return EXIT_SUCCESS;
}
