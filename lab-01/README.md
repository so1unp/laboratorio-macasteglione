# Laboratorio 1 - El Lenguaje de Programación C

El objetivo de este laboratorio es familiarizarse con el lenguaje C y las herramientas de desarrollo. Como editor de texto se utilizará VSCode y se empleará el compilador desde la línea de comandos. Para el control de versiones se empleará [Git](https://git-scm.com/) y GitHub para administrar los repositorios remotos. [Aquí](http://rogerdudler.github.io/git-guide/index.es.html) pueden leer una guía sencilla para aprender los comandos básicos de Git.

### Compilación

Para compilar los programas vamos a utilizar el comando `make`. Si se ejecuta sin parámetros, compila _todos_ los programas del laboratorio. Si se indica el nombre de un archivo _sin la extensión_ compila sólo el programa indicado.

Por ejemplo, para compilar el ejercicio 1 (programa `sum.c`):

```
$ make sum
gcc -g -Wall -Wextra -Wshadow -Wconversion -Wunreachable-code -o ./bin/sum sum.c
sum.c: In function ‘main’:
sum.c:4:14: warning: unused parameter ‘argc’ [-Wunused-parameter]
    4 | int main(int argc, char *argv[])
      |          ~~~~^~~~
sum.c:4:26: warning: unused parameter ‘argv’ [-Wunused-parameter]
    4 | int main(int argc, char *argv[])
      |                    ~~~~~~^~~~~~
$
```

El compilador genera una serie de _warnings_ (advertencias). En este caso, nos está indicando que los parámetros `argc` y `argv` de la función `main()` no estan siendo utilizados (`unused parameter`). El objetivo es que la resolución de cada ejercicio cumpla con el enunciado y que _además_ compile _sin advertencias_.

El ejecutable se crea dentro del subdirectorio `bin` del laboratorio. Lo podemos comprobar haciendo un listado del contenido de dicho directorio:

```
$ ls bin
sum
$
```

Para ejecutar el programa recien compilado:

```
$ bin/sum
$
```

Como todavía no se realizó una modifación al programa, no realiza ninguna acción.

## Ejercicio 0

El programa `demo.c` es una demostración de las funcionalidades básicas del lenguaje C: declaración de variables, estructuras, ciclos, control, punteros e imprimir mensajes por la salida estándar haciendo uso de funciones de la biblioteca. Leer el código, compilar el programa y ejecutarlo.

## Ejercicio 1

Completar el programa `hola.c` para que imprima por la salida estandar el mensaje indicado como parámetro. Utilizar la función [`printf()`](https://www.man7.org/linux/man-pages/man3/printf.3.html) para imprimir el mensaje. Un ejemplo de ejecución es:

```
$ bin/hola "hola mundo"
hola mundo
$
```

Para obtener el texto a imprimir se debe utilizar el argumento `argv[]` de la función `main()`. No olvidar verificar que exista el argumento inspeccionando `argc`.

## Ejercicio 2

Modificar el programa `sum.c` para que retorne la suma de un número arbitrario de enteros indicados desde la línea de comandos. Por ejemplo:

```
$ bin/sum 1 2 3
6
$
```

Utilizar la funcion [`atoi()`](https://www.man7.org/linux/man-pages/man3/atoi.3.html) para convertir las cadenas de texto a números enteros.

## Ejercicio 3

Completar el programa `palabras.c` para que imprima las palabras que recibe desde la _entrada estándar_. Por ejemplo:

```
$ bin/palabras
Esto es un texto ↵
Esto
es
un
texto
Hola mundo ↵
Hola
mundo
^C
$
```

En el ejemplo, luego de ingresar una frase y presionar Enter (`↵`), el programa imprime cada palabra, una por línea. El programa finaliza al presionar `^C` (Ctrl+C).

Utilizar la función [`getchar()`](https://www.man7.org/linux/man-pages/man3/getchar.3.html) para obtener un carácter desde la _entrada estándar_.

## Ejercicio 4

Completar el programa `histograma.c` para que imprima un histograma de las longitudes de las palabras de su entrada. La versión más sencilla del histograma es con las barras horizontales. La orientación vertical es más desafiante. El texto a analizar se debe leer desde la _entrada estándar_. Un ejemplo de ejecución sería:

```
$ bin/histograma
Esto es un texto ↵
Esto es otro texto ↵
^D
1
2 ***
3
4 ***
5 **
$

```

En el ejemplo se ejecuta el programa, que queda a la espera de datos desde la entrada estándar. Se ingresan las frases `Esto es un texto` y `Esto es otro texto`, cada una seguida de un _Enter_. Luego se presiona `^D` (Ctrl+D), combinación que envía un `EOF`.

El histograma indica que en el texto analizado se encontraron tres palabras con dos letras (_es_ dos veces y _un_), tres palabras de cuatro letras y dos de cinco.

---

¡Fín del Laboratorio 1!
