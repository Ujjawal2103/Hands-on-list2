
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>

int main() {
    int shmid;
    char *ptr;
    volatile char *readonly_ptr;
    pid_t pid;
    int status;

    // Create a shared memory segment
    shmid = shmget(IPC_PRIVATE, 1024, IPC_CREAT | 0600);

    if (shmid == -1) {
        perror("shmget");
        return EXIT_FAILURE;
    }

    printf("Shared memory created. ID: %d\n", shmid);

    // Attach with read-write access
    ptr = (char *)shmat(shmid, NULL, 0);

    if (ptr == (void *)-1) {
        perror("shmat");
        shmctl(shmid, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }

    // Write data
    strcpy(ptr, "Hello from shared memory!");
    printf("Written data: %s\n", ptr);

    // Detach the read-write attachment
    if (shmdt(ptr) == -1) {
        perror("shmdt");
        shmctl(shmid, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }

    printf("Read-write attachment detached.\n");

    // Attach in read-only mode
    readonly_ptr = (volatile char *)shmat(
        shmid, NULL, SHM_RDONLY
    );

    if (readonly_ptr == (void *)-1) {
        perror("shmat read-only");
        shmctl(shmid, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }

    printf("Read-only data: %s\n", (const char *)readonly_ptr);

    // Test writing in a child process so a fault
    // does not terminate the main program.
    pid = fork();

    if (pid == -1) {
        perror("fork");
        shmdt((const void *)readonly_ptr);
        shmctl(shmid, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        readonly_ptr[0] = 'X';  // Expected to cause SIGSEGV
        _exit(EXIT_SUCCESS);
    }

    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid");
        shmdt((const void *)readonly_ptr);
        shmctl(shmid, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }

    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGSEGV) {
        printf("Write blocked: read-only attachment cannot be modified.\n");
    } else {
        printf("Unexpected result while testing read-only access.\n");
    }

    // Detach read-only attachment
    if (shmdt((const void *)readonly_ptr) == -1) {
        perror("shmdt read-only");
        shmctl(shmid, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }

    printf("Read-only attachment detached.\n");

    // Remove the shared memory segment
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl IPC_RMID");
        return EXIT_FAILURE;
    }

    printf("Shared memory removed successfully.\n");

    return EXIT_SUCCESS;
}
