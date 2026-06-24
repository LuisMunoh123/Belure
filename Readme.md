# BELURE

**Universidad de Magallanes — Facultad de Ingeniería**
**Asignatura:** Diseño de Algoritmos — 2026

**Integrantes:** Benjamin Sanhueza, Renato Uribe y Luis Muñoz.
**Docente:** [Jacqueline Aldridge Águila](mailto:jacqueline.aldridge@umag.cl)

---

## Descripción

BELURE es un programa desarrollado en C para generar, administrar y analizar una base de datos ficticia de deportistas.

El sistema permite generar datos, leer archivos CSV, ordenar deportistas, ejecutar algoritmos de búsqueda, obtener rankings, encontrar el k-ésimo mejor deportista y realizar experimentos de rendimiento.

Además, incorpora estrategias **Greedy** y **Programación Dinámica** para resolver un problema de selección de deportistas con restricción de presupuesto.

Este proyecto toma como base el trabajo realizado en [PALYER](https://github.com/AndrewhiteCode/PALYER), ampliándolo con nuevos algoritmos, experimentos y funcionalidades.

---

## Objetivo

Implementar y comparar algoritmos de ordenamiento, búsqueda, selección, estrategias Greedy y Programación Dinámica utilizando datos ficticios de deportistas.

El proyecto busca relacionar el análisis teórico de complejidad con resultados experimentales obtenidos al aumentar la cantidad de datos procesados.

---

## Funcionamiento general

Cada deportista posee información como identificador, nombre, equipo, puntaje y cantidad de competencias. Para las funcionalidades de selección con presupuesto, también se considera un costo asociado a cada deportista.

El sistema puede utilizarse de la siguiente forma:

1. Se genera una base de datos de deportistas en formato CSV.
2. Los datos pueden ordenarse según distintos criterios.
3. Se puede buscar un deportista por identificador, puntaje o rango de puntajes.
4. Se pueden obtener rankings y seleccionar el k-ésimo mejor deportista.
5. Se pueden ejecutar experimentos para comparar algoritmos.
6. Se puede seleccionar un conjunto de deportistas usando Greedy o Programación Dinámica.

En el escenario con presupuesto, el objetivo es obtener el mayor puntaje total posible sin superar un presupuesto máximo.

Cada deportista se selecciona completo o no se selecciona, por lo que el problema se modela como una variante de la mochila 0/1.

[
\max \sum_{i=1}^{n} puntaje_i \cdot x_i
]

Sujeto a:

[
\sum_{i=1}^{n} costo_i \cdot x_i \leq presupuesto
]

donde:

* (x_i = 1) si el deportista es seleccionado.
* (x_i = 0) si el deportista no es seleccionado.

---

## Requisitos

Para compilar y ejecutar el proyecto se recomienda utilizar Linux, WSL o un entorno compatible con herramientas POSIX.

Se necesita contar con:

* Compilador GCC.
* `make`.
* Terminal Bash o similar.
* Git, para clonar el repositorio.

En Ubuntu o WSL se pueden instalar las herramientas necesarias con:

```sh
sudo apt update
sudo apt install build-essential git
```

---

## Instalación

Clonar el repositorio:

```sh
git clone https://github.com/LuisMunoh123/Belure.git
cd Belure
```

Crear las carpetas requeridas por el proyecto:

```sh
make folders
```

Compilar el programa:

```sh
make
```

El ejecutable se genera en:

```text
build/belure.out
```

Para eliminar archivos compilados y datos generados:

```sh
make clean
```

> **Importante:** `make clean` puede eliminar archivos dentro de `build/`, incluyendo resultados CSV generados por experimentos. Guarda una copia de los resultados importantes antes de ejecutar este comando.

---

## Flujo recomendado de uso

Primero se genera una base de datos de deportistas:

```sh
./build/belure.out -g 2000 -t shuffled
```

Luego se puede revisar el contenido del archivo actual:

```sh
./build/belure.out -r
```

Después se pueden ejecutar funciones de ordenamiento, búsqueda, selección o experimentación.

Por ejemplo, para obtener una solución óptima con presupuesto mediante tabulación:

```sh
./build/belure.out -d tabulation -b 3000
```

Para ejecutar las estrategias Greedy configuradas en el programa:

```sh
./build/belure.out -G
```

Para ejecutar los experimentos disponibles:

```sh
./build/belure.out -e
```

La mayoría de las funcionalidades se ejecuta directamente mediante banderas de línea de comandos. La opción `-e` abre un menú interno para escoger el experimento disponible.

---

## Estructura principal del proyecto

| Ruta                        | Descripción                                                                            |
| --------------------------- | -------------------------------------------------------------------------------------- |
| `src/main.c`                | Procesa las banderas de línea de comandos y dirige la ejecución de cada funcionalidad. |
| `src/greedy.c`              | Implementa las estrategias Greedy para la selección de deportistas.                    |
| `src/dynamic_programming.c` | Implementa Programación Dinámica mediante memoización y tabulación.                    |
| `src/team_experiment.c`     | Ejecuta experimentos de comparación entre Programación Dinámica y Greedy.              |
| `incs/`                     | Contiene archivos de cabecera, estructuras, constantes y prototipos de funciones.      |
| `build/`                    | Contiene los archivos compilados del proyecto.                                         |
| `build/db/`                 | Guarda bases de datos CSV y resultados de experimentos.                                |
| `docs/results/`             | Contiene gráficos y resultados visuales generados a partir de experimentos.            |
| `Makefile`                  | Define los comandos de compilación, limpieza y creación de carpetas.                   |

---

## Referencia de comandos

```text
./build/belure.out -g <cantidad> -t <tipo>            Generar CSV
./build/belure.out -r                                 Leer CSV actual
./build/belure.out -s -a <algoritmo> -c <criterio>    Ordenar deportistas
./build/belure.out -f <tipo_busqueda> -i <id>         Buscar por ID
./build/belure.out -j <k>                             K-ésimo mejor deportista
./build/belure.out -p <N>                             Ranking de los N mejores
./build/belure.out -q <puntaje>                       Buscar por puntaje exacto
./build/belure.out -R -m <min> -M <max>               Buscar por rango de puntaje
./build/belure.out -e                                 Ejecutar experimento
./build/belure.out -G                                 Ejecutar estrategias Greedy
./build/belure.out -d tabulation -b <presupuesto>     Programación Dinámica por tabulación
./build/belure.out -d memoization -b <presupuesto>    Programación Dinámica por memoización
./build/belure.out -h                                 Mostrar ayuda
```

---

## Opciones detalladas

| Bandera                              | Descripción                                                          |
| ------------------------------------ | -------------------------------------------------------------------- |
| `-g, --generate <cantidad>`          | Genera un CSV con la cantidad indicada de deportistas.               |
| `-t, --type <tipo>`                  | Define el tipo de generación: `sorted`, `inverse` o `shuffled`.      |
| `-r, --read`                         | Lee e imprime la base de datos CSV actual.                           |
| `-s, --sort`                         | Ordena los deportistas cargados en memoria.                          |
| `-a, --algorithm <algoritmo>`        | Define el algoritmo de ordenamiento.                                 |
| `-c, --criteria <criterio>`          | Define el criterio utilizado para ordenar.                           |
| `-f, --find <tipo_busqueda>`         | Busca un deportista mediante su identificador.                       |
| `-i, --id <id>`                      | Indica el identificador del deportista que se desea buscar.          |
| `-j, --kth <k>`                      | Muestra el k-ésimo mejor deportista mediante Quick Select.           |
| `-p, --topcount <N>`                 | Muestra un ranking de los N mejores deportistas.                     |
| `-q, --score <puntaje>`              | Busca deportistas con un puntaje exacto.                             |
| `-R, --score-range`                  | Activa la búsqueda por rango de puntaje.                             |
| `-m, --min-score <min>`              | Define el puntaje mínimo de una búsqueda por rango.                  |
| `-M, --max-score <max>`              | Define el puntaje máximo de una búsqueda por rango.                  |
| `-e, --experiment`                   | Muestra el menú de experimentos disponibles.                         |
| `-G, --greedy`                       | Ejecuta las estrategias Greedy configuradas en el programa.          |
| `-d, --dynamic-programming <metodo>` | Ejecuta Programación Dinámica mediante `tabulation` o `memoization`. |
| `-b, --budget <presupuesto>`         | Define el presupuesto máximo para Programación Dinámica.             |
| `-h, --help`                         | Muestra la ayuda general del programa.                               |

---

## Algoritmos de ordenamiento

Los algoritmos de ordenamiento disponibles son:

```text
swap
insertion
selection
cocktail
quick
merge
merge-optimized
```

Los criterios disponibles para ordenar son:

```text
id
name
team
score
competitions
```

Ejemplo: ordenar por puntaje usando Quick Sort.

```sh
./build/belure.out -s -a quick -c score
```

Ejemplo: ordenar por nombre usando Merge Sort optimizado.

```sh
./build/belure.out -s -a merge-optimized -c name
```

---

## Algoritmos de búsqueda

Las búsquedas por identificador disponibles son:

```text
linear
binary
binary-recursive
exponential
interpolation
```

Ejemplo: buscar al deportista con identificador 42 mediante búsqueda exponencial.

```sh
./build/belure.out -f exponential -i 42
```

Ejemplo: buscar al deportista con identificador 25 mediante búsqueda por interpolación.

```sh
./build/belure.out -f interpolation -i 25
```

---

## Selección y ranking

### K-ésimo mejor deportista

La opción `-j` permite obtener un deportista según su posición de puntaje, utilizando Quick Select.

```sh
./build/belure.out -j 3
```

El comando anterior muestra el tercer mejor deportista según puntaje.

### Ranking de los N mejores

La opción `-p` muestra un ranking con los mejores deportistas.

```sh
./build/belure.out -p 10
```

El comando anterior muestra el ranking de los 10 mejores deportistas.

### Búsqueda por puntaje exacto

```sh
./build/belure.out -q 80
```

### Búsqueda por rango de puntaje

```sh
./build/belure.out -R -m 80 -M 95
```

El comando anterior muestra los deportistas cuyo puntaje se encuentra entre 80 y 95.

---

## Selección de deportistas con presupuesto

BELURE implementa dos enfoques para seleccionar deportistas bajo un presupuesto máximo:

1. Programación Dinámica.
2. Estrategias Greedy.

### Programación Dinámica

La Programación Dinámica compara combinaciones posibles de deportistas para obtener el mayor puntaje total sin superar el presupuesto.

Se implementan dos versiones:

* `tabulation`: construye una tabla de resultados desde los subproblemas más pequeños.
* `memoization`: resuelve subproblemas de forma recursiva y guarda los resultados ya calculados.

Ejemplo con tabulación:

```sh
./build/belure.out -d tabulation -b 3000
```

Ejemplo con memoización:

```sh
./build/belure.out -d memoization -b 3000
```

Ambas versiones resuelven el mismo modelo de mochila 0/1 y buscan obtener el puntaje óptimo.

### Estrategias Greedy

Las estrategias Greedy construyen una solución rápida ordenando primero a los deportistas según un criterio y luego agregando los que todavía caben en el presupuesto.

Los criterios implementados son:

| Estrategia          | Criterio                                                         |
| ------------------- | ---------------------------------------------------------------- |
| Mayor puntaje       | Prioriza deportistas con puntajes individuales más altos.        |
| Menor costo         | Prioriza deportistas de menor costo.                             |
| Razón puntaje/costo | Prioriza mayor puntaje por cada unidad de presupuesto utilizada. |

La razón puntaje/costo se calcula como:

[
\frac{puntaje}{costo}
]

Ejecutar las estrategias Greedy:

```sh
./build/belure.out -G
```

> **Importante:** las estrategias Greedy son rápidas, pero no garantizan siempre el puntaje óptimo en un problema de selección 0/1. La razón puntaje/costo puede entregar buenos resultados prácticos, aunque también puede fallar al no poder seleccionar fracciones de un deportista.

---

## Escenario sin presupuesto

El proyecto también considera un escenario sin restricción de costo: seleccionar exactamente los K deportistas con mayor puntaje.

En este caso, ordenar los deportistas por puntaje descendente y escoger los primeros K es correcto, porque no existe un presupuesto u otra restricción que pueda modificar la selección.

Este escenario se analiza de forma independiente del problema con presupuesto, por lo que no se mezcla con los resultados comparativos de Programación Dinámica y Greedy con presupuesto.

---

## Ejemplos de uso

### Generar 5000 deportistas en orden aleatorio

```sh
./build/belure.out -g 5000 -t shuffled
```

### Leer la base de datos actual

```sh
./build/belure.out -r
```

### Ordenar por puntaje con Quick Sort

```sh
./build/belure.out -s -a quick -c score
```

### Ordenar por nombre con Merge Sort optimizado

```sh
./build/belure.out -s -a merge-optimized -c name
```

### Buscar al deportista con ID 42 usando búsqueda exponencial

```sh
./build/belure.out -f exponential -i 42
```

### Ver el tercer mejor deportista

```sh
./build/belure.out -j 3
```

### Ver el ranking de los 10 mejores

```sh
./build/belure.out -p 10
```

### Buscar deportistas con puntaje entre 80 y 95

```sh
./build/belure.out -R -m 80 -M 95
```

### Ejecutar estrategias Greedy

```sh
./build/belure.out -G
```

### Ejecutar Programación Dinámica por tabulación con presupuesto 3000

```sh
./build/belure.out -d tabulation -b 3000
```

### Ejecutar Programación Dinámica por memoización con presupuesto 3000

```sh
./build/belure.out -d memoization -b 3000
```

---

## Paginación de resultados

Cuando el programa debe mostrar listas grandes de deportistas, utiliza paginación para evitar saturar la consola.

* Presionar `Enter` muestra los siguientes 20 elementos.
* Presionar `q` permite salir de la paginación.

---

## Experimentación

La opción `-e` permite ejecutar los experimentos disponibles en el proyecto.

```sh
./build/belure.out -e
```

Para el experimento de Programación Dinámica y Greedy, se comparan:

* Programación Dinámica por tabulación.
* Programación Dinámica por memoización.
* Greedy por mayor puntaje.
* Greedy por menor costo.
* Greedy por razón puntaje/costo.

Los resultados se guardan en archivos CSV dentro de `build/db/`.

| Archivo                                | Contenido                                    |
| -------------------------------------- | -------------------------------------------- |
| `build/db/team_experiment_raw.csv`     | Resultados individuales de cada repetición.  |
| `build/db/team_experiment_summary.csv` | Promedios por tamaño de entrada y algoritmo. |

Los indicadores principales del experimento son:

* Tiempo promedio de ejecución.
* Puntaje promedio obtenido.
* Diferencia porcentual de Greedy respecto del óptimo de Programación Dinámica.

La diferencia porcentual se calcula mediante:

[
\frac{puntaje_PD - puntaje_Greedy}{puntaje_PD} \cdot 100
]

Una diferencia de `0%` indica que Greedy obtuvo el mismo puntaje que Programación Dinámica para esa instancia. Una diferencia alta indica que Greedy encontró una solución de menor calidad.

Los gráficos generados a partir de los archivos CSV pueden almacenarse en:

```text
docs/results/
```

---

## Manejo de errores

La función `print_error`, ubicada en `src/errors.c`, se encarga de mostrar los mensajes de error del programa.

| Código | Descripción                                                                           |
| -----: | ------------------------------------------------------------------------------------- |
|  `100` | File `<target>` could not be read.                                                    |
|  `101` | File `<target>` could not be opened.                                                  |
|  `102` | It was not possible to allocate memory for `<target>`.                                |
|  `103` | The number you entered exceeds the maximum value allowed for `<target>`.              |
|  `104` | You must specify exactly one action.                                                  |
|  `105` | To generate data, use `-g <amount>` and `-t <generate type>`.                         |
|  `106` | Invalid sort algorithm.                                                               |
|  `107` | Invalid sort criteria.                                                                |
|  `108` | Invalid search algorithm.                                                             |
|  `109` | To search, use `-f [search type] -i <id>`.                                            |
|  `110` | Unsupported search algorithm.                                                         |
|  `111` | To select, use `-j <k>`.                                                              |
|  `112` | To rank, use `-p <N>`.                                                                |
|  `113` | For score ranges, use `-R -m <min> -M <max>`.                                         |
|  `114` | The minimum score cannot be greater than the maximum score.                           |
|  `115` | To search for an exact score, use `-q <score>`.                                       |
|  `116` | Invalid option.                                                                       |
|  `301` | The number of players is too high; the console will show only the first `%d` players. |

---

## Créditos

Proyecto desarrollado por:

* Benjamin Sanhueza
* Renato Uribe
* Luis Muñoz

Universidad de Magallanes
Facultad de Ingeniería
Diseño de Algoritmos — 2026
