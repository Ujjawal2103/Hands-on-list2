
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int main() {
    int binary_sem, counting_sem;
    union semun arg;

    // Create a semaphore for binary semaphore
    binary_sem = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);

    if (binary_sem == -1) {
        perror("semget binary");
        return EXIT_FAILURE;
    }

    // Initialize binary semaphore to 1
    arg.val = 1;

    if (semctl(binary_sem, 0, SETVAL, arg) == -1) {
        perror("semctl binary");
        semctl(binary_sem, 0, IPC_RMID);
        return EXIT_FAILURE;
    }

    printf("Binary semaphore created.\n");
    printf("Binary semaphore value: %d\n",
           semctl(binary_sem, 0, GETVAL));

    // Create a semaphore for counting semaphore
    counting_sem = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);

    if (counting_sem == -1) {
        perror("semget counting");
        semctl(binary_sem, 0, IPC_RMID);
        return EXIT_FAILURE;
    }

    // Initialize counting semaphore to 5
    arg.val = 5;

    if (semctl(counting_sem, 0, SETVAL, arg) == -1) {
        perror("semctl counting");
        semctl(binary_sem, 0, IPC_RMID);
        semctl(counting_sem, 0, IPC_RMID);
        return EXIT_FAILURE;
    }

    printf("Counting semaphore created.\n");
    printf("Counting semaphore value: %d\n",
           semctl(counting_sem, 0, GETVAL));

    // Remove both semaphore sets
    if (semctl(binary_sem, 0, IPC_RMID) == -1) {
        perror("remove binary semaphore");
        semctl(counting_sem, 0, IPC_RMID);
        return EXIT_FAILURE;
    }

    if (semctl(counting_sem, 0, IPC_RMID) == -1) {
        perror("remove counting semaphore");
        return EXIT_FAILURE;
    }

    printf("Both semaphores removed successfully.\n");

    return EXIT_SUCCESS;
}
