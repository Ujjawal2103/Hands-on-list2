
#include <stdio.h>
#include <sys/resource.h>

void print_limit(const char *name, int resource)
{
    struct rlimit limit;

    if (getrlimit(resource, &limit) == -1) {
        perror(name);
        return;
    }

    printf("%-15s Soft limit: ", name);

    if (limit.rlim_cur == RLIM_INFINITY)
        printf("Unlimited");
    else
        printf("%llu", (unsigned long long)limit.rlim_cur);

    printf("\tHard limit: ");

    if (limit.rlim_max == RLIM_INFINITY)
        printf("Unlimited");
    else
        printf("%llu", (unsigned long long)limit.rlim_max);

    printf("\n");
}

int main(void)
{
    printf("System Resource Limits\n");
    printf("-----------------------------------------------\n");

    print_limit("CPU", RLIMIT_CPU);
    print_limit("File size", RLIMIT_FSIZE);
    print_limit("Data size", RLIMIT_DATA);
    print_limit("Stack size", RLIMIT_STACK);
    print_limit("Core file", RLIMIT_CORE);
    print_limit("Open files", RLIMIT_NOFILE);
    print_limit("Processes", RLIMIT_NPROC);
    print_limit("Address space", RLIMIT_AS);

    return 0;
}
