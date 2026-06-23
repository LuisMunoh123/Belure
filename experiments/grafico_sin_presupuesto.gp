set datafile separator ","
set terminal pngcairo size 1400,850 enhanced font "Arial,14"
set output "docs/results/greedy_top_k_sin_presupuesto.png"
set title "Greedy sin presupuesto: tiempo para seleccionar Top K"
set xlabel "Cantidad de deportistas (n)"
set ylabel "Tiempo promedio (microsegundos)"
set grid
set key outside right center
set logscale x

plot "build/db/no_budget_greedy_summary.csv" using 1:4 with linespoints title "Greedy Top K"
