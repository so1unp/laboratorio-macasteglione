#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
  // Agregar codigo aqui

  int ch;
  while ((ch = getchar()) != EOF)
    if (isalpha(ch))
      putchar(ch);
    else
      putchar('\n');

  // Termina la ejecución del programa.
  exit(EXIT_SUCCESS);
}
