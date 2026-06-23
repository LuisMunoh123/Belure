set terminal pngcairo size 1200,700
set output "docs/results/prueba_png.png"

set title "Prueba de Gnuplot"
set xlabel "Eje X"
set ylabel "Eje Y"
set grid

plot "-" using 1:2 with linespoints lw 2 pt 7 ps 1.5 title "Datos de prueba"
1 1
2 4
3 2
4 6
5 5
e
