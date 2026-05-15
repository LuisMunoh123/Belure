# Conquer

**Universidad de Magallanes - Diseño de Algoritmos - 2026**

**Estudiantes:** [Franco Aguilar](mailto:fagucarc@umag.cl), [Milton Hernández](mailto:mhern@umag.cl), [Luis Muñoz](mailto:luimuoz@umag.cl).

**Docente:** [Jacqueline Aldridge ](mailto:jacqueline.aldridge@umag.cl)

El presente repositorio tiene como objetivo la comparación y comprobación de los órdenes de complejidad de distintos algoritmos recursivos de ordenamiento, búsqueda y selección, mediante su implementación y evaluación sobre datos artificialmente generados de deportistas.

El sistema se desarrolla en lenguaje C y considera la generación de registros que sirven como base para el análisis teórico y experimental del comportamiento de los algoritmos en distintos tamaños de entrada.

Este proyecto toma como base el trabajo realizado en [PALYER](https://github.com/AndrewhiteCode/PALYER), expandiendo su funcionalidad para incluir algoritmos de la estrategia Divide y Vencerás.

# Objetivo
El objetivo del presente repositorio es implementar algoritmos de ordenamiento, búsqueda y selección basados en la estrategia Divide y Vencerás, contrastando su rendimiento sobre una base de datos ficticia de deportistas.

# Instalación y uso
Una vez clonado el repositorio, se espera el siguiente flujo de comandos:
1. `make folders`: Crea la estructura de carpetas necesaria para el funcionamiento del programa.
2. `make`: Compila el proyecto.

El programa se controla completamente desde la línea de comandos. No existe un menú interactivo: cada operación se invoca directamente con sus banderas correspondientes.

## Referencia de comandos
```
./build/conquer.out -g <cantidad> -t <tipo>            Generar CSV
./build/conquer.out -r                                 Leer CSV actual
./build/conquer.out -s -a <algoritmo> -c <criterio>    Ordenar CSV
./build/conquer.out -f <tipo_búsqueda> -i <id>         Buscar por ID
./build/conquer.out -j <k>                             K-ésimo mejor deportista
./build/conquer.out -p <N>                             Ranking de los N mejores
./build/conquer.out -q <puntaje>                       Buscar por puntaje exacto
./build/conquer.out -R -m <min> -M <max>               Deportistas en rango de puntaje
./build/conquer.out -e                                 Ejecutar experimento
./build/conquer.out -h                                 Mostrar ayuda
```

### Opciones detalladas
| Bandera | Descripción |
|---|---|
| `-g, --generate <cantidad>` | Genera un CSV con la cantidad indicada de deportistas |
| `-t, --type <tipo>` | Tipo de generación: `sorted`, `inverse`, `shuffled` |
| `-r, --read` | Lee e imprime el CSV actual |
| `-s, --sort` | Ordena el CSV en memoria |
| `-a, --algorithm <algoritmo>` | Algoritmo de ordenamiento: `swap`, `insertion`, `selection`, `cocktail`, `quick`, `merge`, `merge-optimized` |
| `-c, --criteria <criterio>` | Criterio de ordenamiento: `id`, `name`, `team`, `score`, `competitions` |
| `-f, --find <tipo_búsqueda>` | Busca un deportista por ID: `linear`, `binary`, `binary-recursive`, `exponential`, `interpolation` |
| `-i, --id <id>` | ID del deportista a buscar |
| `-j, --kth <k>` | Muestra el k-ésimo mejor deportista (Quick Select) |
| `-p, --topcount <N>` | Muestra el ranking de los N mejores deportistas |
| `-q, --score <puntaje>` | Busca deportistas con un puntaje exacto |
| `-R, --score-range` | Activa la búsqueda por rango de puntaje |
| `-m, --min-score <min>` | Puntaje mínimo del rango |
| `-M, --max-score <max>` | Puntaje máximo del rango |
| `-e, --experiment` | Ejecuta el experimento y guarda los resultados en `build/db/` |
| `-h, --help` | Muestra la ayuda |

## Ejemplos de uso

**Generar 5000 deportistas en orden aleatorio:**
```sh
./build/conquer.out -g 5000 -t shuffled
```

**Ordenar por puntaje con Quick Sort:**
```sh
./build/conquer.out -s -a quick -c score
```

**Ordenar por nombre con Merge Sort optimizado:**
```sh
./build/conquer.out -s -a merge-optimized -c name
```

**Buscar al deportista con ID 42 usando búsqueda exponencial:**
```sh
./build/conquer.out -f exponential -i 42
```

**Ver el 3er mejor deportista:**
```sh
./build/conquer.out -j 3
```

**Ver el ranking de los 10 mejores:**
```sh
./build/conquer.out -p 10
```

**Ver deportistas con puntaje entre 80 y 95:**
```sh
./build/conquer.out -R -m 80 -M 95
```

> [!IMPORTANT]
> El programa incluye un sistema de paginación para mostrar cómodamente listas de más de 20 deportistas. Se usa `Enter` para avanzar a los siguientes 20 elementos y `q` para salir de la paginación y continuar con la ejecución.

## Experimento
La bandera `-e` ejecuta el experimento de rendimiento sobre todos los algoritmos implementados. Los resultados quedan almacenados en `build/db/experiment.csv` y pueden graficarse con:
```sh
make plot
```

El experimento evalúa cada algoritmo en múltiples tamaños de entrada y repite cada medición varias veces para reducir la varianza. Los parámetros del experimento (número de pasos y repeticiones por paso) se configuran en `incs/generate_exec_times.h`. Tras modificarlos, recompilar con:
```sh
make clean && make
```

## Generación de gráficos
El repositorio incluye una utilidad en Python para visualizar los resultados experimentales.
- `make install-allegro6`: Instala las dependencias necesarias. Si el entorno actual no es compatible, el instalador puede crear un entorno virtual dentro de `allegro6/`.
- `make plot`: Procesa `build/db/experiment.csv` y genera una imagen PNG y una animación GIF en `docs/results/`.

## Manejo de errores
Dentro del archivo [errors.c](src/errors.c) se encuentra la función `print_error` que se encarga de manejar los errores que puedan ocurrir en el programa. Los códigos de error que se manejan son los siguientes:

| Código de error | Descripción                                                              |
| --------------- | -------------------------------------------------------------------------|
|100              | File <target> could not be read                                          | 
|101              | File <target> could not be opened                                        |
|102              | It was not possible to allocate memory for <target>                      |103              | The number you entered exceeds the maximum value allowed for <target>     |
|104              | You must specify exactly one action                                      |
|105              | To generate data, use -g <amount> and -t <generate type>                 |
|106              | Invalid sort algorithm                                                   |
|107              | Invalid sort criteria                                                    |
|108              | Invalid search algorithm                                                 |
|109              | To search, use -f [search type] -i <id>                                  |
|110              | Unsupported search algorithm                                             |
|111              | To select, use -j <k>                                                    |
|112              | To rank, use -p <N>                                                      |
|113              | For score ranges, use -R -m <min> -M <max>                               |
|114              | The minimum score cannot be greater than the maximum score               |
|115              | To search for an exact score, use -q <score>                             |
|116              | Invalid option                                                           |
|301              | The number of players is too high, console will show the first %d players|
