# BELURE

**Universidad de Magallanes - Diseño de Algoritmos - 2026**

**Estudiantes:** Benjamin Sanhueza, Renato Uribe y Luis Muñoz.

**Docente:** [Jacqueline Aldridge ](mailto:jacqueline.aldridge@umag.cl)

BELURE es un programa en C para probar y comparar algoritmos usando una base de datos ficticia de deportistas.

El sistema permite generar datos, ordenarlos, buscar deportistas, seleccionar rankings y ejecutar experimentos. También incluye estrategias greedy y programación dinámica para resolver problemas con presupuesto.

Este proyecto toma como base el trabajo realizado en [PALYER](https://github.com/AndrewhiteCode/PALYER), agregando nuevas funciones y algoritmos.

# Objetivo
Implementar y comparar algoritmos de ordenamiento, búsqueda, selección, greedy y programación dinámica sobre datos de deportistas.

# Instalación y uso
Una vez clonado el repositorio, se espera el siguiente flujo de comandos:
1. `make folders`: Crea la estructura de carpetas necesaria para el funcionamiento del programa.
2. `make`: Compila el proyecto.

El programa se controla completamente desde la línea de comandos. No existe un menú interactivo: cada operación se invoca directamente con sus banderas correspondientes.

## Referencia de comandos
```
./build/belure.out -g <cantidad> -t <tipo>            Generar CSV
./build/belure.out -r                                 Leer CSV actual
./build/belure.out -s -a <algoritmo> -c <criterio>    Ordenar CSV
./build/belure.out -f <tipo_búsqueda> -i <id>         Buscar por ID
./build/belure.out -j <k>                             K-ésimo mejor deportista
./build/belure.out -p <N>                             Ranking de los N mejores
./build/belure.out -q <puntaje>                       Buscar por puntaje exacto
./build/belure.out -R -m <min> -M <max>               Deportistas en rango de puntaje
./build/belure.out -e                                 Ejecutar experimento
./build/belure.out -G                                 Ejecutar estrategias Greedy
./build/belure.out -d tabulation -b <presupuesto>     Programación dinámica por tabulación
./build/belure.out -d memoization -b <presupuesto>    Programación dinámica por memoización
./build/belure.out -h                                 Mostrar ayuda
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
| `-G, --greedy` | Ejecuta las estrategias greedy: mayor score, menor costo, mejor score/costo y sin presupuesto |
| `-d, --dynamic-programming <método>` | Ejecuta programación dinámica: `tabulation` o `memoization` |
| `-b, --budget <presupuesto>` | Define el presupuesto máximo para programación dinámica |
| `-h, --help` | Muestra la ayuda |

## Ejemplos de uso

**Generar 5000 deportistas en orden aleatorio:**
```sh
./build/belure.out -g 5000 -t shuffled
```

**Ordenar por puntaje con Quick Sort:**
```sh
./build/belure.out -s -a quick -c score
```

**Ordenar por nombre con Merge Sort optimizado:**
```sh
./build/belure.out -s -a merge-optimized -c name
```

**Buscar al deportista con ID 42 usando búsqueda exponencial:**
```sh
./build/belure.out -f exponential -i 42
```

**Ver el 3er mejor deportista:**
```sh
./build/belure.out -j 3
```

**Ver el ranking de los 10 mejores:**
```sh
./build/belure.out -p 10
```

**Ver deportistas con puntaje entre 80 y 95:**
```sh
./build/belure.out -R -m 80 -M 95
```

**Ejecutar estrategias greedy:**
```sh
./build/belure.out -G
```

**Ejecutar programación dinámica por tabulación con presupuesto 5000:**
```sh
./build/belure.out -d tabulation -b 5000
```

**Ejecutar programación dinámica por memoización con presupuesto 5000:**
```sh
./build/belure.out -d memoization -b 5000
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
