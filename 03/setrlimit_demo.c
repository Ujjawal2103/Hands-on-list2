
#include <stdio.h>
#include <sys/resource.h>

int main(void)
{
    struct rlimit limit;

    if (getrlimit(RLIMIT_NOFILE, &limit) == -1) {
        perror("getrlimit");
        return 1;
    }

    printf("Before changing the limit:\n");
    printf("Soft limit = %llu\n", (unsigned long long)limit.rlim_cur);
    printf("Hard limit = %llu\n", (unsigned long long)limit.rlim_max);

    limit.rlim_cur = 64;

    if (setrlimit(RLIMIT_NOFILE, &limit) == -1) {
        perror("setrlimit");
        return 1;
    }

    if (getrlimit(RLIMIT_NOFILE, &limit) == -1) {
        perror("getrlimit");
        return 1;
    }

    printf("\nAfter changing the limit:\n");
    printf("Soft limit = %llu\n", (unsigned long long)limit.rlim_cur);
    printf("Hard limit = %llu\n", (unsigned long long)limit.rlim_max);

    return 0;
}
