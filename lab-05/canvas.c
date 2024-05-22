#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define HEIGHT 25 // Altura en caracteres de la pizarra
#define WIDTH 25  // Ancho en caracteres de la pizarra

struct canvas
{
    char canvas[HEIGHT * WIDTH];
};

typedef struct canvas canvas_t;

void usage(char *argv[])
{
    fprintf(stderr, "Uso: %s comando parametro\n", argv[0]);
    fprintf(stderr, "Comandos:\n");
    fprintf(stderr, "\t-w canvas mensaje x y [h|v]: escribe el mensaje en el canvas indicado en la posición (x,y) de manera [h]orizontal o [v]ertical.\n");
    fprintf(stderr, "\t-p canvas: imprime el canvas indicado.\n");
    fprintf(stderr, "\t-c canvas: crea un canvas con el nombre indicado.\n");
    fprintf(stderr, "\t-d canvas: elimina el canvas indicado.\n");
    fprintf(stderr, "\t-h imprime este mensaje.\n");
}

int crear_canvas(const char *name)
{
    int fd = shm_open(name, O_CREAT | O_RDWR, 0666);

    if (fd == -1)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd, sizeof(canvas_t)) == -1)
    {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    canvas_t *canvas = mmap(NULL, sizeof(canvas_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (canvas == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        exit(EXIT_FAILURE);
    }

    memset(canvas->canvas, ' ', sizeof(canvas->canvas));
    munmap(canvas, sizeof(canvas_t));
    close(fd);

    return 0;
}

int borrar_canvas(const char *name)
{
    if (shm_unlink(name) == -1)
    {
        perror("shm_unlink");
        exit(EXIT_FAILURE);
    }

    return 0;
}

int mostrar_canvas(const char *name)
{
    int fd = shm_open(name, O_RDONLY, 0666), i, j;

    if (fd == -1)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    canvas_t *canvas = mmap(NULL, sizeof(canvas_t), PROT_READ, MAP_SHARED, fd, 0);

    if (canvas == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
            putchar(canvas->canvas[i * WIDTH + j]);
        putchar('\n');
    }

    munmap(canvas, sizeof(canvas_t));
    close(fd);

    return 0;
}

int escribir_canvas(const char *name, const char *mensaje, int x, int y, char direccion)
{
    int fd = shm_open(name, O_RDWR, 0666);
    size_t i;

    if (fd == -1)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    canvas_t *canvas = mmap(NULL, sizeof(canvas_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (canvas == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        exit(EXIT_FAILURE);
    }

    size_t len = strlen(mensaje);

    if (direccion == 'h')
        if (x >= 0 && x < HEIGHT && y >= 0 && y + len <= WIDTH)
            for (i = 0; i < len; i++)
                canvas->canvas[x * WIDTH + y + i] = mensaje[i];
        else
            fprintf(stderr, "Error: coordenadas fuera de rango.\n");

    else if (direccion == 'v')
        if (x >= 0 && x + len < HEIGHT && y >= 0 && y < WIDTH)
            for (i = 0; i < len; i++)
                canvas->canvas[(x + i) * WIDTH + y] = mensaje[i];
        else
            fprintf(stderr, "Error: coordenadas fuera de rango.\n");
    else
        fprintf(stderr, "Error: dirección desconocida '%c'. Use 'h' para horizontal o 'v' para vertical.\n", direccion);

    munmap(canvas, sizeof(canvas_t));
    close(fd);

    return 0;
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
        printf("Escribe %s en el canvas %s en la posición (%d, %d).\n", argv[3], argv[2], atoi(argv[4]), atoi(argv[5]));

        if (argc != 7)
        {
            usage(argv);
            exit(EXIT_FAILURE);
        }

        escribir_canvas(argv[2], argv[3], atoi(argv[4]), atoi(argv[5]), argv[6][0]);
        break;

    case 'p':
        printf("Imprime canvas.\n");

        if (argc != 3)
        {
            usage(argv);
            exit(EXIT_FAILURE);
        }

        mostrar_canvas(argv[2]);
        break;

    case 'c':
        printf("Crea canvas.\n");

        if (argc != 3)
        {
            usage(argv);
            exit(EXIT_FAILURE);
        }

        crear_canvas(argv[2]);
        break;

    case 'd':
        printf("Borra canvas.\n");

        if (argc != 3)
        {
            usage(argv);
            exit(EXIT_FAILURE);
        }

        borrar_canvas(argv[2]);
        break;

    case 'h':
        usage(argv);
        break;

    default:
        fprintf(stderr, "Comando desconocido\n");
        usage(argv);
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
