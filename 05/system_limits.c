
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("System Configuration Limits\n");
    printf("----------------------------\n");

    printf("1. Maximum argument length: %ld bytes\n",
           sysconf(_SC_ARG_MAX));

    printf("2. Maximum processes per user ID: %ld\n",
           sysconf(_SC_CHILD_MAX));

    printf("3. Clock ticks per second: %ld\n",
           sysconf(_SC_CLK_TCK));

    printf("4. Maximum open files per process: %ld\n",
           sysconf(_SC_OPEN_MAX));

    printf("5. Page size: %ld bytes\n",
           sysconf(_SC_PAGESIZE));

    printf("6. Total physical memory pages: %ld\n",
           sysconf(_SC_PHYS_PAGES));

    printf("7. Available physical memory pages: %ld\n",
           sysconf(_SC_AVPHYS_PAGES));

    return 0;
}
