#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

void *thread_function(void *arg);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Uso: %s <número de hilos> <tiempo máximo de espera>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int num_threads = atoi(argv[1]), max_sleep = atoi(argv[2]);
    srand(time(NULL));

    pthread_t threads[num_threads];
    int i;

    for (i = 0; i < num_threads; i++)
    {
        if (pthread_create(&threads[i], NULL, thread_function, &max_sleep) != 0)
        {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < num_threads; i++)
    {
        void *result;
        if (pthread_join(threads[i], &result) != 0)
        {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
        printf("Hilo %lu terminó: %d\n", pthread_self(), *(int *)result);
        free(result);
    }

    exit(EXIT_SUCCESS);
}

void *thread_function(void *arg)
{
    int max_sleep = *(int *)arg;
    int sleep_time = rand() % max_sleep + 1;

    printf("Hilo %lu: dormirá %d segundos\n", pthread_self(), sleep_time);

    sleep(sleep_time);

    int *result = malloc(sizeof(int));
    *result = sleep_time;

    pthread_exit(result);
}