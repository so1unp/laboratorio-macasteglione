#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h> // times()
#include <unistd.h>

int busywork(void)
{
    struct tms buf;
    for (;;)
        times(&buf);
}

int main(int argc, char *argv[])
{
    int childs = atoi(argv[1]), pid;

    for (int i = 1; i < childs; i++)
        pid = fork();

    exit(EXIT_SUCCESS);
}
