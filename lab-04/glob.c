#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

static long glob = 0;
static sem_t sem;

void *thread_function(void *arg);
void increment_glob(int loops);

int main(int argc, char *argv[])
{
    int loops;
    pthread_t thread1, thread2;

    // Controla numero de argumentos.
    if (argc != 2)
    {
        fprintf(stderr, "Uso: %s ciclos\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    loops = atoi(argv[1]);

    // Verifica argumentos.
    if (loops < 1)
    {
        fprintf(stderr, "Error: ciclos debe ser mayor a cero.\n");
        exit(EXIT_FAILURE);
    }

    if (sem_init(&sem, 0, 1) != 0)
    {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&thread1, NULL, thread_function, &loops) != 0 ||
        pthread_create(&thread2, NULL, thread_function, &loops) != 0)
    {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    if (pthread_join(thread1, NULL) != 0 || pthread_join(thread2, NULL) != 0)
    {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }

    sem_destroy(&sem);
    printf("%ld\n", glob);

    exit(EXIT_SUCCESS);
}

void *thread_function(void *arg)
{
    int loops = *(int *)arg;
    increment_glob(loops);
    pthread_exit(NULL);
}

void increment_glob(int loops)
{
    int loc, j;

    sem_wait(&sem);

    for (j = 0; j < loops; j++)
    {
        loc = glob;
        loc++;
        glob = loc;
    }

    sem_post(&sem);
}