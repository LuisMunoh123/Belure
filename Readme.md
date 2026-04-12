# Proyecto 1 — Algoritmos iterativos

**Universidad de Magallanes - Diseño de Algoritmos - 2026**
**Estudiantes:** [Andres Barbosa](mailto:abarbosa@umag.cl), [Milton Hernández](mailto:mhern@umag.cl), [Iván Gallardo](mailto:ivangall@umag.cl).
**Docente:** [Jacqueline Aldridge ](mailto:jacqueline.aldridge@umag.cl)

El presente repositorio tiene como objetivo la comparación y comprobación de los órdenes de complejidad de distintos algoritmos de ordenamiento y búsqueda, mediante su implementación y evaluación sobre datos artificialmente generados de deportistas.

El sistema se desarrolla en lenguaje C y considera la generación de registros con atributos como ID, nombre, equipo, puntaje y cantidad de competencias, los cuales servirán como base para el análisis teórico y experimental del comportamiento de los algoritmos en distintos tamaños de entrada.

En esta primera etapa, el repositorio está orientado a la construcción de la base del sistema, incluyendo la generación y almacenamiento de datos. La explicación detallada de los algoritmos de ordenamiento, búsqueda y su implementación será incorporada progresivamente a medida que el proyecto avance.

## Objetivo general

Desarrollar un sistema que permita generar, almacenar, ordenar y buscar información de deportistas, con el fin de analizar empíricamente y contrastar teóricamente el rendimiento de distintos algoritmos clásicos de ordenamiento y búsqueda.

> [!NOTE] Nota
> Este README corresponde a una versión inicial del proyecto. La documentación será ampliada conforme se implementen las funcionalidades y algoritmos requeridos.

## Generación de gráficos y animaciones

El repositorio incluye una utilidad en Python para visualizar los resultados experimentales a partir del archivo `experiment.csv`.

### Comandos disponibles

- `make install-allegro6`: instala las dependencias necesarias para la utilidad de visualización. Si el entorno actual no es adecuado, el instalador puede crear un entorno virtual dentro de `allegro6/`.
- `make plot`: ejecuta la utilidad de graficación y genera una animación GIF junto con una imagen PNG del frame final.

### Estructura relacionada

La funcionalidad asociada a `make plot` se encuentra encapsulada en la carpeta `allegro6/`, donde se ubican el script principal en Python, el archivo de dependencias y el instalador correspondiente a sistemas **UNIX**.

### Salida esperada

Al ejecutar `make plot`, el programa procesa los datos experimentales y genera archivos gráficos que permiten visualizar la evolución y comparación de los algoritmos analizados en `docs/results/`.

## Manejo de errores

Dentro del archivo [errors.c](src/errors.c) se encuentra la función `print_error` que se encarga de manejar los errores que puedan ocurrir en el programa. Los códigos de error que se manejan son los siguientes:

| Código de error | Descripción                                        |
| --------------- | ---------------------------------------------------|
|100              | File <target> could not be read                    |
|101              | File <target> could not be opened                  |
|102              | It was not possible to allocate memory for <target>|
|301              | The number of players is too high, console will show the first MAX_CONSOLE_READABLE_PLAYERS players|
