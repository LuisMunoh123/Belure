# Conquer

**Universidad de Magallanes - Diseño de Algoritmos - 2026**

**Estudiantes:** [Franco Aguilar](mailto:fagucarc@umag.cl), [Milton Hernández](mailto:mhern@umag.cl), [Luis Muñoz](mailto:luimuoz@umag.cl).

**Docente:** [Jacqueline Aldridge ](mailto:jacqueline.aldridge@umag.cl)

El presente repositorio tiene como objetivo la comparación y comprobación de los órdenes de complejidad de distintos algoritmos recursivos de ordenamiento, búsqueda y selección, mediante su implementación y evaluación sobre datos artificialmente generados de deportistas.

El sistema se desarrolla en lenguaje C y considera la generación de registros, los cuales servirán como base para el análisis teórico y experimental del comportamiento de los algoritmos en distintos tamaños de entrada.

Este proyecto toma como base el trabajo realizado en el proyecto [PALYER](https://github.com/AndrewhiteCode/PALYER) expandiendo su funcionalidad para incluir algoritmos recursivos.

<!--
TODO: Lo que sigue a partir de aquí hay que modificarlo en base al nuevo proyecto.


## Objetivo general

Desarrollar un sistema que permita generar, almacenar, ordenar y buscar información de deportistas, con el fin de analizar empíricamente y contrastar teóricamente el rendimiento de distintos algoritmos clásicos de ordenamiento y búsqueda.

## Instalación y uso de la aplicación.
Una vez clonado el repositorio se espera el siguiente flujo de comandos:
1. `make folders`: Creará la estructura de carpetas necesarias para el funcionamiento del programa.
2. `make`: Compilará el programa.
3. `make run`: Ejecutará el programa.

Una vez ejecutado el programa el usuario se enfrentará a un menú de 6 opciones; finalizada la ejecución del programa, deberá de ejecutar nuevamente `make run` para volver al menú.

A continuación se detallan las funcionalidades del programa:
1. **Generate CSV**: Permite generar un archivo CSV con los datos de entrada del programa, que será utilizado para la ejecución del experimento. El archivo queda almacenado en la carpeta `build/db/`.
2. **Read CSV**: Permite leer el archivo CSV generado en el paso anterior.
3. **Sort CSV**: Permite ordenar el archivo CSV bajo 4 algoritmos distintos. (La ordenación se realiza en memoria, no se guarda en disco). El ordenamiento se puede realizar por cualquiera de los campos que componen un deportista (ID, nombre, equipo, puntaje, competencias).
4. **Search CSV**: Permite buscar un jugador en el archivo CSV bajo 2 algoritmos distintos conocido su ID.
5. **Run experiment**: Permite ejecutar el experimento de ordenamiento y búsqueda. El resultado del experimento se almacena en la carpeta `build/db/` y se puede graficar con `make plot` (como se detalla en la secicón siguiente).
    - El experimento tiene tres variables que pueden ser modificadas: el número de deportistas (Seleccionado al momento de generar el CSV), el número de pasos de la prueba y la cantidad de veces que cada algoritmo se ejecuta por paso. Las últimas dos variables se encuentran en el archivo `incs/generate_exec_times.h`. Luego de modificar las variables, se debe ejecutar `make clean && make && make run` para recompilar el programa exitosamente.


> [!important]
> El programa incluye un sistema de paginación para mostrar cómodamente una lista de deportistas mayor a $20$ elementos. Para navegar por ella se usa `enter` para avanzar a los siguientes $20$ elementos y `q` para salir de la paginación y continuar con la ejecución del programa.

## Generación de gráficos y animaciones

El repositorio incluye una utilidad en Python para visualizar los resultados experimentales a partir del archivo `experiment.csv`.

### Comandos disponibles

- `make install-allegro6`: instala las dependencias necesarias para la utilidad de visualización. Si el entorno actual no es adecuado, el instalador puede crear un entorno virtual dentro de `allegro6/`.
- `make plot`: ejecuta la utilidad de graficación y genera una animación GIF junto con una imagen PNG del frame final.

### Estructura relacionada

La funcionalidad asociada a `make plot` se encuentra encapsulada en la carpeta `allegro6/`, donde se ubican el script principal en Python, el archivo de dependencias y el instalador correspondiente a sistemas **UNIX**.

### Salida esperada

Al ejecutar `make plot`, el programa procesa los datos experimentales y genera archivos gráficos que permiten visualizar la evolución y comparación de los algoritmos analizados en `docs/results/`.

-->

## Manejo de errores

Dentro del archivo [errors.c](src/errors.c) se encuentra la función `print_error` que se encarga de manejar los errores que puedan ocurrir en el programa. Los códigos de error que se manejan son los siguientes:

| Código de error | Descripción                                        |
| --------------- | ---------------------------------------------------|
|100              | File <target> could not be read                    |
|101              | File <target> could not be opened                  |
|102              | It was not possible to allocate memory for <target>|
|103              | The number you entered exceeds the maximum value allowed for <target>|
|301              | The number of players is too high, console will show the first MAX_CONSOLE_READABLE_PLAYERS players|
