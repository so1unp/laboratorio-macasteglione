#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define ITEMS 15
#define MAX_WORD 50

struct wordstack
{
    int free;
    int items;
    int max_word;
    char heap[ITEMS][MAX_WORD];
    pthread_mutex_t mutex;
    sem_t full;
    sem_t empty;
};

typedef struct wordstack wordstack_t;

void usage(char *argv[])
{
    fprintf(stderr, "Uso: %s comando parametro\n", argv[0]);
    fprintf(stderr, "Comandos:\n");
    fprintf(stderr, "\t-w pila palabra: agrega palabra en la pila\n");
    fprintf(stderr, "\t-r pila pos: elimina la palabra de la pila.\n");
    fprintf(stderr, "\t-p pila: imprime la pila de palabras.\n");
    fprintf(stderr, "\t-c pila: crea una zona de memoria compartida con el nombre indicado donde almacena la pila.\n");
    fprintf(stderr, "\t-d pila: elimina la pila indicada.\n");
    fprintf(stderr, "\t-h imprime este mensaje.\n");
}

void crear_pila(const char *name)
{
    int fd = shm_open(name, O_CREAT | O_RDWR, 0666);

    if (fd == -1)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd, sizeof(wordstack_t)) == -1)
    {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    wordstack_t *stack = mmap(NULL, sizeof(wordstack_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (stack == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    stack->free = ITEMS;
    stack->items = 0;
    stack->max_word = MAX_WORD;

    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&stack->mutex, &mutex_attr);
    pthread_mutexattr_destroy(&mutex_attr);

    sem_init(&stack->full, 1, 0);
    sem_init(&stack->empty, 1, ITEMS);

    if (munmap(stack, sizeof(wordstack_t)) == -1)
    {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    close(fd);
    printf("Pila de palabras creada.\n");
}

void borrar_pila(const char *name)
{
    int fd = shm_open(name, O_RDWR, 0666);

    if (fd == -1)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    wordstack_t *stack = mmap(NULL, sizeof(wordstack_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (stack == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    sem_destroy(&stack->full);
    sem_destroy(&stack->empty);
    pthread_mutex_destroy(&stack->mutex);

    if (munmap(stack, sizeof(wordstack_t)) == -1)
    {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    close(fd);

    if (shm_unlink(name) == -1)
    {
        perror("shm_unlink");
        exit(EXIT_FAILURE);
    }

    printf("Pila de palabras eliminada.\n");
}

void agregar_letra(const char *name, const char *letra)
{
    int fd = shm_open(name, O_RDWR, 0666);

    if (fd == -1)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    wordstack_t *stack = mmap(NULL, sizeof(wordstack_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (stack == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    sem_wait(&stack->empty);
    pthread_mutex_lock(&stack->mutex);

    if (stack->items < ITEMS)
    {
        strncpy(stack->heap[stack->items], letra, MAX_WORD - 1);
        stack->heap[stack->items][MAX_WORD - 1] = '\0';
        stack->items++;
        stack->free--;
        printf("Palabra '%s' agregada a la pila.\n", letra);
    }

    pthread_mutex_unlock(&stack->mutex);
    sem_post(&stack->full);

    if (munmap(stack, sizeof(wordstack_t)) == -1)
    {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    close(fd);
}

void quitar_letra(const char *name)
{
    int fd = shm_open(name, O_RDWR, 0666);

    if (fd == -1)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    wordstack_t *stack = mmap(NULL, sizeof(wordstack_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (stack == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    sem_wait(&stack->full);
    pthread_mutex_lock(&stack->mutex);

    if (stack->items > 0)
    {
        stack->items--;
        stack->free++;
        printf("Palabra '%s' eliminada de la pila.\n", stack->heap[stack->items]);
    }

    pthread_mutex_unlock(&stack->mutex);
    sem_post(&stack->empty);

    if (munmap(stack, sizeof(wordstack_t)) == -1)
    {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    close(fd);
}

void mostrar_pila(const char *name)
{
    int fd = shm_open(name, O_RDWR, 0666);
    int i;

    if (fd == -1)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    wordstack_t *stack = mmap(NULL, sizeof(wordstack_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (stack == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_lock(&stack->mutex);

    printf("Pila de palabras:\n");
    for (i = 0; i < stack->items; i++)
        printf("%d: %s\n", i, stack->heap[i]);

    pthread_mutex_unlock(&stack->mutex);

    if (munmap(stack, sizeof(wordstack_t)) == -1)
    {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    if (argv[1][0] != '-')
    {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    char option = argv[1][1];

    switch (option)
    {
    case 'w':
        if (argc != 4)
        {
            usage(argv);
            exit(EXIT_FAILURE);
        }

        agregar_letra(argv[2], argv[3]);
        break;

    case 'r':
        if (argc != 3)
        {
            usage(argv);
            exit(EXIT_FAILURE);
        }

        quitar_letra(argv[2]);
        break;

    case 'p':
        if (argc != 3)
        {
            usage(argv);
            exit(EXIT_FAILURE);
        }

        mostrar_pila(argv[2]);
        break;

    case 'c':
        if (argc != 3)
        {
            usage(argv);
            exit(EXIT_FAILURE);
        }

        crear_pila(argv[2]);
        break;

    case 'd':
        if (argc != 3)
        {
            usage(argv);
            exit(EXIT_FAILURE);
        }

        borrar_pila(argv[2]);
        break;

    case 'h':
        usage(argv);
        break;

    default:
        fprintf(stderr, "-%c: opción desconocida.\n", option);
        usage(argv);
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
