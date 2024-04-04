#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Debe ingresar los datos correctamente: <PID> <SEÑAL>\n");
        exit(EXIT_FAILURE);
    }

    int pid = atoi(argv[1]), signal = atoi(argv[2]);

    kill(pid, signal);
    printf("Envie la señal '%s' al PID %d\n", strsignal(signal), pid);

    exit(EXIT_SUCCESS);
}