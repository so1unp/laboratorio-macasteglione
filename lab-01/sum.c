#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  //Agregar codigo aqui

  int suma = 0;

  for (int i = 1; argv[i] != NULL; i++)
    suma += atoi(argv[i]);
  
  printf("%d\n", suma);

  //Termina la ejecución del programa.
  exit(EXIT_SUCCESS);
}
