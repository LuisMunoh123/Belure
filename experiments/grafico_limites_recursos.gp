set datafile separator ","
set terminal pngcairo size 1400,850 enhanced font "Arial,14"
set output "docs/results/limites_recursos.png"
set title "Limite practico de ejecucion por algoritmo"
set xlabel "Cantidad de deportistas (n)"
set ylabel "Tiempo de ejecucion (microsegundos)"
set grid
set key outside right center
set logscale x

plot \
"build/db/team_resource_limits.csv" using 1:(strcol(2) eq "greedy_ratio" ? $3 : 1/0) with linespoints title "Greedy ratio", \
"build/db/team_resource_limits.csv" using 1:(strcol(2) eq "dp_tabulation" ? $3 : 1/0) with linespoints title "DP tabulacion", \
"build/db/team_resource_limits.csv" using 1:(strcol(2) eq "dp_memoization" ? $3 : 1/0) with linespoints title "DP memoizacion"
