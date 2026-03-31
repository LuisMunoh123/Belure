# Ordenador de Registros de Deportistas

Este es un programa de línea de comandos escrito en C que permite ordenar un registro de deportistas (estructuras) basándose en diferentes criterios y direcciones (ascendente o descendente). Utiliza la función `qsort` de la biblioteca estándar de C y punteros a funciones para lograr una ordenación dinámica y eficiente.

## Características

* Ordenamiento dinámico desde la consola mediante argumentos (`argc` y `argv`).
* Soporte para múltiples tipos de datos:
    * Enteros (`int`): Para IDs y cantidad de competencias.
    * Cadenas de texto (`char[]`): Para ordenamiento alfabético de nombres y equipos.
    * Punto flotante (`double`): Para ordenamiento preciso de puntajes.
* Implementación escalable: Fácil de extender para nuevos campos de la estructura `Deportista`.

## Requisitos previos

Para compilar y ejecutar este proyecto, necesitas tener instalado un compilador de C, como **GCC**, y una terminal funcional.

## Instalación y Compilación

1. Clona este repositorio en tu entorno local:
   ```bash
   git clone [https://github.com/AndrewhiteCode/ordenador-deportistas.git](https://github.com/AndrewhiteCode/ordenador-deportistas.git)