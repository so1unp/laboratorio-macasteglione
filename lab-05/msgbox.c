#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

#define USERNAME_MAXSIZE 15 // Máximo tamaño en caracteres del nombre del remitente.
#define TXT_SIZE 100        // Máximo tamaño del texto del mensaje.

/**
 * Estructura del mensaje:
 * - sender: nombre del usuario que envió el mensaje.
 * - text: texto del mensaje.
 */
struct msg
{
    char sender[USERNAME_MAXSIZE];
    char text[TXT_SIZE];
};

typedef struct msg msg_t;

/**
 * Imprime información acerca del uso del programa.
 */
void usage(char *argv[])
{
    fprintf(stderr, "Uso: %s comando parametro\n", argv[0]);
    fprintf(stderr, "Comandos:\n");
    fprintf(stderr, "\t-s queue mensaje: escribe el mensaje en queue.\n");
    fprintf(stderr, "\t-r queue: imprime el primer mensaje en queue.\n");
    fprintf(stderr, "\t-a queue: imprime todos los mensaje en queue.\n");
    fprintf(stderr, "\t-l queue: vigila por mensajes en queue.\n");
    fprintf(stderr, "\t-c queue: crea una cola de mensaje queue.\n");
    fprintf(stderr, "\t-d queue: elimina la cola de mensajes queue.\n");
    fprintf(stderr, "\t-h imprime este mensaje.\n");
}

void crear_cola(const char *name)
{
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(msg_t);
    attr.mq_curmsgs = 0;

    mqd_t cola = mq_open(name, O_CREAT | O_RDWR, 0666, &attr);

    if (cola == (mqd_t)-1)
    {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    mq_close(cola);
}

void eliminar_cola(const char *name)
{
    if (mq_unlink(name) == -1)
    {
        perror("mq_unlink");
        exit(EXIT_FAILURE);
    }
}

void enviar_mensaje(const char *name, const char *texto)
{
    mqd_t cola = mq_open(name, O_WRONLY);

    if (cola == (mqd_t)-1)
    {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    msg_t mensaje;

    if (getlogin_r(mensaje.sender, USERNAME_MAXSIZE) != 0)
    {
        perror("getlogin_r");
        mq_close(cola);
        exit(EXIT_FAILURE);
    }

    strncpy(mensaje.text, texto, TXT_SIZE - 1);
    mensaje.text[TXT_SIZE - 1] = '\0';

    if (mq_send(cola, (char *)&mensaje, sizeof(msg_t), 0) == -1)
    {
        perror("mq_send");
        mq_close(cola);
        exit(EXIT_FAILURE);
    }

    mq_close(cola);
}

void recibir_mensaje(const char *name)
{
    mqd_t cola = mq_open(name, O_RDONLY);

    if (cola == (mqd_t)-1)
    {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    msg_t mensaje;

    if (mq_receive(cola, (char *)&mensaje, sizeof(msg_t), NULL) == -1)
    {
        perror("mq_receive");
        mq_close(cola);
        exit(EXIT_FAILURE);
    }

    printf("%s: %s\n", mensaje.sender, mensaje.text);

    mq_close(cola);
}

void mostrar_mensajes_todos(const char *name)
{
    mqd_t cola = mq_open(name, O_RDONLY);

    if (cola == (mqd_t)-1)
    {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    msg_t mensaje;

    while (mq_receive(cola, (char *)&mensaje, sizeof(msg_t), NULL) != -1)
        printf("%s: %s\n", mensaje.sender, mensaje.text);

    if (errno != EAGAIN)
        perror("mq_receive");

    mq_close(cola);
}

void esperar_mensajes(const char *name)
{
    mqd_t cola = mq_open(name, O_RDONLY);

    if (cola == (mqd_t)-1)
    {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    msg_t mensaje;

    printf("Esperando mensajes en la cola %s..\n", name);
    while (1)
    {
        if (mq_receive(cola, (char *)&mensaje, sizeof(msg_t), NULL) == -1)
        {
            perror("mq_receive");
            mq_close(cola);
            exit(EXIT_FAILURE);
        }
        printf("%s: %s\n", mensaje.sender, mensaje.text);
    }

    mq_close(cola);
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
    case 's':
        printf("Enviar %s a la cola %s\n", argv[3], argv[2]);

        if (argc != 4)
        {
            usage(argv);
            exit(EXIT_FAILURE);
        }

        enviar_mensaje(argv[2], argv[3]);
        break;

    case 'r':
        printf("Recibe el primer mensaje en %s\n", argv[2]);

        if (argc != 3)
        {
            usage(argv);
            exit(EXIT_FAILURE);
        }

        recibir_mensaje(argv[2]);
        break;

    case 'a':
        printf("Imprime todos los mensajes en %s\n", argv[2]);

        if (argc != 3)
        {
            usage(argv);
            exit(EXIT_FAILURE);
        }

        mostrar_mensajes_todos(argv[2]);
        break;

    case 'l':
        printf("Escucha indefinidamente por mensajes\n");

        if (argc != 3)
        {
            usage(argv);
            exit(EXIT_FAILURE);
        }

        esperar_mensajes(argv[2]);
        break;

    case 'c':
        printf("Crea la cola de mensajes %s\n", argv[2]);

        if (argc != 3)
        {
            usage(argv);
            exit(EXIT_FAILURE);
        }

        crear_cola(argv[2]);
        break;

    case 'd':
        printf("Borra la cola de mensajes %s\n", argv[2]);

        if (argc != 3)
        {
            usage(argv);
            exit(EXIT_FAILURE);
        }

        eliminar_cola(argv[2]);
        break;

    case 'h':
        usage(argv);
        break;

    default:
        fprintf(stderr, "Comando desconocido: %s\n", argv[1]);
        usage(argv);
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
