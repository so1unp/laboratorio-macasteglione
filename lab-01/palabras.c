#include <stdio.h>
#include <stdlib.h>

int main()
{
    // Agregar codigo aqui

    int ch;
    int estaEnPalabra = 0;

    while ((ch = getchar()) != EOF)
        if (ch != ' ' && ch != '\t')
            if (!estaEnPalabra)
            {
                estaEnPalabra = 1;
                putchar(ch);
            }
            else
                putchar(ch);
        else
        {
            if (estaEnPalabra)
                putchar('\n');
            estaEnPalabra = 0;
        }

    // Termina la ejecución del programa.
    exit(EXIT_SUCCESS);
}
