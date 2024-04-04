#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void handler(int sign)
{
    printf("Recibi la señal: %s\n", strsignal(sign));
}

int main(void)
{
    int signalId;

    printf("Mi PID es: %d\n", getpid());

    for (signalId = 1; signalId < 255; signalId++)
        signal(signalId, handler);

    while (1)
        pause();

    exit(EXIT_SUCCESS);
}
