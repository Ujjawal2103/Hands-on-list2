
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int change_sem(int semid, int sem_num, int change) {
    struct sembuf op;

    op.sem_num = sem_num;
    op.sem_op = change;
    op.sem_flg = 0;

    return semop(semid, &op, 1);
}

int main() {
    int semid, shmid;
    int *ticket;
    union semun arg;
    unsigned short values[2] = {1, 2};
    pid_t pid;

    // Create two semaphores
    semid = semget(IPC_PRIVATE, 2, IPC_CREAT | 0600);
    if (semid == -1) {
        perror("semget");
        return EXIT_FAILURE;
    }

    // Initialize mutex = 1 and counting semaphore = 2
    arg.array = values;
    if (semctl(semid, 0, SETALL, arg) == -1) {
        perror("semctl SETALL");
        semctl(semid, 0, IPC_RMID);
        return EXIT_FAILURE;
    }

    // Create shared memory for the ticket counter
    shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);
    if (shmid == -1) {
        perror("shmget");
        semctl(semid, 0, IPC_RMID);
        return EXIT_FAILURE;
    }

    ticket = (int *)shmat(shmid, NULL, 0);
    if (ticket == (void *)-1) {
        perror("shmat");
        shmctl(shmid, IPC_RMID, NULL);
        semctl(semid, 0, IPC_RMID);
        return EXIT_FAILURE;
    }

    *ticket = 0;

    // Create four child processes
    for (int i = 0; i < 4; i++) {
        pid = fork();

        if (pid == -1) {
            perror("fork");
            break;
        }

        if (pid == 0) {
            // Acquire one of the two available resources
            if (change_sem(semid, 1, -1) == -1)
                _exit(EXIT_FAILURE);

            // Each child issues 10000 tickets
            for (int j = 0; j < 10000; j++) {
                // Enter critical section
                if (change_sem(semid, 0, -1) == -1)
                    _exit(EXIT_FAILURE);

                (*ticket)++;

                // Leave critical section
                if (change_sem(semid, 0, 1) == -1)
                    _exit(EXIT_FAILURE);
            }

            // Release the resource
            if (change_sem(semid, 1, 1) == -1)
                _exit(EXIT_FAILURE);

            _exit(EXIT_SUCCESS);
        }
    }

    // Wait for all children
    while (wait(NULL) > 0)
        ;

    printf("Final ticket count: %d\n", *ticket);
    printf("Expected ticket count: 40000\n");

    // Clean up resources
    shmdt(ticket);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);

    printf("Shared memory and semaphores removed.\n");

    return EXIT_SUCCESS;
}
