/*
 * Este programa presenta la funcionalidad basica del lenguaje C.
 */

/* Incluye la libreria stdio, que contiene las funciones puts(), printf(), etc */
#include <stdio.h>
#include <stdlib.h>

/* Definición de una estructura de datos. */
struct punto
{
    int x;
    int y;
};

/* Definición de una variable global. */
int variable_global;

/* Definición de un arreglo con valores */
int arreglo[5] = {0, 1, 2, 3, 4};

/* Otro arreglo, pero que cuyo contenido no esta definido. */
int buffer[2048];

/*
 * Definición de una función:
 * - Require un entero como parámetro.
 * - Retorna un entero.
 */
int cuadrado(int x)
{
    return x * x;
}

/*
 * función main()
 *
 * Esta es la función donde comienza la ejecución del programa.
 *
 * argc: número de parámetros con que fue invocado el programa.
 * argv: arreglo con punteros a cada uno de los parámetros.
 *
 * Probar invocar el programa de esta manera: ./democ 1
 */
int main(int argc, char *argv[])
{
    /* Estas son variables locales de la función main(). */
    char var1 = 65;             // byte (8 bits)
    int var2 = 2023;            // Entero
    float var3 = 0.101f;        // Punto flotante
    double var4 = 3.1415926535; // Punto flotante (mayor precisión)

    /* Variable de tipo struct punto */
    struct punto punto;

    punto.x = 13;
    punto.y = 10;

    /* Actualiza el valor de la variable global. */
    variable_global = 0xAB;

    /* Invoca una función y guardamos el resultado en una variable. */
    int potencia = cuadrado(var2);

    /* Imprime una cadena por pantalla, invocando la función puts(). */
    puts("hola mundo!");

    /* Condicionales */
    if (argc > 1)
    {
        // Hay al menos un parámetro (el nombre del programa es el 1er parametro).
        // %s indica que se imprime una cadena
        printf("Parametro %s\n", argv[1]);
    }

    /* Imprime las variables locales.
     * %c formatea como caracter ascii.
     * %d formatea como numero entero.
     * %f formatea como numero flotante.
     * \n nueva linea.
     */
    printf("%X\n%c\n%d\n%2.4f\n%2.8f\n", variable_global, var1, var2, var3, var4);
    printf("var2^2 %d\n", potencia);
    printf("var3^2 %d\n", cuadrado(var3)); // !!! Perdida de precisión (¿por que?)

    /* Uso de la estructura */
    if (punto.x > punto.y)
    {
        puts("Eje X");
    }
    else
    {
        puts("Eje Y");
    }

    /* Loops */
    int i = 0;

    /* Ejecuta repetidamente el bloque hasta que la condición de while() sea falsa. */
    while (i < 5)
    {
        printf("while %d\n", i);
        i++; // equivalente a i = i + 1
    }

    /* Ejecuta repetidamente el bloque el número de veces indicado. */
    for (i = 0; i < 5; i++)
    {
        printf("for %d\n", arreglo[i]);
    }

    char *p; /* Puntero a una variable de tipo entero. */

    p = &var1;

    printf("Dirección de memoria de var1: %p\n", &var1);
    printf("Dirección de memoria de p: %p\n", (void *)&p);
    printf("Valor de la variable p: %p\n", p);
    printf("Valor de la variable a la que apunta p: %c\n", *p); // ---> ¿Por qué imprime 'A'?

    /* malloc(): funcion de librería que reserva memoria dinamicamente */
    /* Reservamos 1 KiB (1024 bytes) */
    p = (char *)malloc(sizeof(char) * 1024);

    /* free(): función de librería que libera memoria */
    free(p);

    /* Retorna un valor al entorno de ejecución, indicando el resultado de la
     * ejecución del programa. La convención es que es cero indica que no hubo
     * errores. */
    return 0;

    /* Otra alternativa, es utilizar la función de biblioteca exit(). Se debe
     * incluir el header <stdlib.h>.
     *
     * exit(EXIT_SUCCESS) en caso de que la ejecución finalice de manera
     * correcta o exit(EXIT_FAILURE) en caso contrario.
     *
     * La ventaja de usar exit() es que se pueden especificar handlers que
     * se ejecuten antes de que el programa finalice.
     */
}
