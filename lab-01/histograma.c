#include <stdio.h>
#include <stdlib.h>

#define MAXIMA_LONGITUD 10

int main()
{
    int palabras[MAXIMA_LONGITUD] = {0}, longitudPalabra = 0; 
    int ch, i, j;

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

    for (i = 1; i < MAXIMA_LONGITUD; i++)
    {
        printf("%d ", i);
        for (j = 0; j < palabras[i]; j++)
            putchar('*');
        putchar('\n');
    }

    exit(EXIT_SUCCESS);
}
