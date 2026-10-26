
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <x86intrin.h>

int main(void)
{
    unsigned int aux;
    unsigned long long start, end;

    volatile pid_t parent_pid;

    _mm_lfence();
    start = __rdtsc();

    for (int i = 0; i < 100; i++) {
        parent_pid = getppid();
    }

    end = __rdtscp(&aux);
    _mm_lfence();

    printf("Number of getppid() calls: 100\n");
    printf("Parent process ID: %d\n", (int)parent_pid);
    printf("Timestamp counter at start: %llu\n", start);
    printf("Timestamp counter at end:   %llu\n", end);
    printf("Total timestamp counter ticks: %llu\n", end - start);
    printf("Average ticks per call: %.2f\n", (double)(end - start) / 100);

    return 0;
}
