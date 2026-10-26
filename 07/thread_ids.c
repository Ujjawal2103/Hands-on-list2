
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_function(void *arg)
{
    printf("Thread ID: %lu\n",
           (unsigned long)pthread_self());

    return NULL;
}

int main(void)
{
    pthread_t threads[3];

    printf("Main thread ID: %lu\n",
           (unsigned long)pthread_self());

    for (int i = 0; i < 3; i++)
    {
        int result = pthread_create(
            &threads[i],
            NULL,
            thread_function,
            NULL
        );

        if (result != 0)
        {
            fprintf(stderr, "Thread creation failed\n");
            exit(EXIT_FAILURE);
        }

        printf("Created thread %d, ID: %lu\n",
               i + 1, (unsigned long)threads[i]);
    }

    for (int i = 0; i < 3; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
