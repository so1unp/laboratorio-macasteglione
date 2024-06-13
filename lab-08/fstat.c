#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

void print_file_type(mode_t mode)
{
    switch (mode & S_IFMT)
    {
    case S_IFREG:
        printf("Tipo de archivo: regular\n");
        break;

    case S_IFDIR:
        printf("Tipo de archivo: directorio\n");
        break;

    case S_IFCHR:
        printf("Tipo de archivo: dispositivo de caracteres\n");
        break;

    case S_IFBLK:
        printf("Tipo de archivo: dispositivo de bloques\n");
        break;

    case S_IFIFO:
        printf("Tipo de archivo: FIFO/pipe\n");
        break;

    case S_IFLNK:
        printf("Tipo de archivo: enlace simbólico\n");
        break;

    case S_IFSOCK:
        printf("Tipo de archivo: socket\n");
        break;

    default:
        printf("Tipo de archivo: desconocido\n");
        break;
    }
}
int main(int argc, char *argv[])
{
    // Agregar código aquí.
    if (argc != 2)
    {
        fprintf(stderr, "Uso: %s <nombre_de_archivo>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct stat file_stat;
    if (stat(argv[1], &file_stat) == -1)
    {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    printf("ID del usuario propietario: %u\n", file_stat.st_uid);
    print_file_type(file_stat.st_mode);
    printf("Número de i-nodo: %lu\n", file_stat.st_ino);
    printf("Tamaño en bytes: %ld\n", file_stat.st_size);
    printf("Número de bloques que ocupa en disco: %ld\n", file_stat.st_blocks);
    printf("Tiempo de última modificación: %s", ctime(&file_stat.st_mtime));
    printf("Tiempo de último acceso: %s", ctime(&file_stat.st_atime));

    // Termina la ejecución del programa.
    exit(EXIT_SUCCESS);
}
