#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h> // times()
#include <unistd.h>
#include <signal.h>
#include <sys/resource.h>

int busywork(void)
{
    struct tms buf;
    for (;;)
        times(&buf);
}

int main(int argc, char *argv[])
{
    unsigned int prioReduce = (unsigned)atoi(argv[3]), tiempo = (unsigned)atoi(argv[2]);
    struct rusage usage;

    if (argc != 4 || prioReduce > 1)
    {
        printf("Debe ingresar los datos correctamente: <CANTIDAD> <TIEMPO> <PRIORIDAD 0 o 1>\n");
        exit(EXIT_FAILURE);
    }

    int childs = atoi(argv[1]), i, hijos[childs];

    printf("Soy el padre con PID: %d\n", getpid());

    for (i = 0; i < childs; i++)
    {
        hijos[i] = fork();

        if (hijos[i] < 0)
        {
            printf("Error al crear un hijo\n");
            exit(EXIT_FAILURE);
        }

        if (hijos[i] == 0)
        {
            getrusage(RUSAGE_SELF, &usage);
            (prioReduce) ? nice(i) : nice(0);
            printf("Child %d (nice %2d):\t%3li\n", getpid(), getpriority(PRIO_PROCESS, (id_t)getpid()), usage.ru_utime.tv_sec);
            busywork();
        }
    }

    (tiempo <= 0) ? (unsigned)pause() : sleep(tiempo);

    for (i = 0; i < childs; i++)
        kill(hijos[i], SIGTERM);

    exit(EXIT_SUCCESS);
}
