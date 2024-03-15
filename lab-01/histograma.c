#include <stdio.h>
#include <stdlib.h>

#define MAXIMA_LONGITUD 10

int main(int argc, char *argv[])
{
    int palabras[MAXIMA_LONGITUD] = {0}, ch, longitudPalabra = 0;

    while ((ch = getchar()) != EOF)
    {
        if (ch != '\t' && ch != ' ' && ch != '\n')
            longitudPalabra++;
        else
        {
            if (longitudPalabra <= MAXIMA_LONGITUD)
                palabras[longitudPalabra]++;
            longitudPalabra = 0;
        }
    }

    for (int i = 1; i < MAXIMA_LONGITUD; i++)
    {
        printf("%d ", i);
        for (int j = 0; j < palabras[i]; j++)
            putchar('*');
        putchar('\n');
    }

    exit(EXIT_SUCCESS);
}
