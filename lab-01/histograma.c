#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXIMA_LONGITUD 10

int main(int argc, char *argv[])
{
    int palabras[MAXIMA_LONGITUD] = {0}, ch, longitudPalabra = 0;

    while ((ch = getchar()) != EOF)
    {
        if (isalpha(ch))
            longitudPalabra++;
        else if (longitudPalabra > 0)
        {
            if (longitudPalabra <= MAXIMA_LONGITUD)
                palabras[longitudPalabra]++;
            longitudPalabra = 0;
        }
    }

    for (int i = 1; i <= MAXIMA_LONGITUD; i++)
    {
        printf("%d ", i);
        for (int j = 0; j < palabras[i]; j++)
            printf("*");
        printf("\n");
    }

    exit(EXIT_SUCCESS);
}
