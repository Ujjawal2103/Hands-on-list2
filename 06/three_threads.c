
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_function(void *arg)
{
    int thread_number = *(int *)arg;

    printf("Thread %d is running\n", thread_number);

    return NULL;
}

int main(void)
{
    pthread_t threads[3];
    int numbers[3] = {1, 2, 3};

    for (int i = 0; i < 3; i++)
    {
        int result = pthread_create(
            &threads[i],
            NULL,
            thread_function,
            &numbers[i]
        );

        if (result != 0)
        {
            printf("Failed to create thread %d\n", i + 1);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < 3; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("All threads have completed\n");

    return 0;
}
