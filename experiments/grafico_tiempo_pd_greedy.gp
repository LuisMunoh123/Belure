set datafile separator ","
set terminal pngcairo size 1400,850 enhanced font "Arial,14"
set output "docs/results/tiempo_pd_greedy.png"
set title "Tiempo promedio de ejecucion: PD vs Greedy"
set xlabel "Cantidad de deportistas (n)"
set ylabel "Tiempo promedio (microsegundos)"
set grid
set key outside right center
# Hace que los tamanos grandes se vean mejor
set logscale x

plot \
"build/db/team_experiment_summary.csv" using 1:(strcol(2) eq "greedy_score" ? $5 : 1/0) with linespoints title "Greedy mayor puntaje", \
"build/db/team_experiment_summary.csv" using 1:(strcol(2) eq "greedy_cost" ? $5 : 1/0) with linespoints title "Greedy menor costo", \
"build/db/team_experiment_summary.csv" using 1:(strcol(2) eq "greedy_ratio" ? $5 : 1/0) with linespoints title "Greedy ratio puntaje/costo", \
"build/db/team_experiment_summary.csv" using 1:(strcol(2) eq "dp_tabulation" ? $5 : 1/0) with linespoints title "DP tabulacion", \
"build/db/team_experiment_summary.csv" using 1:(strcol(2) eq "dp_memoization" ? $5 : 1/0) with linespoints title "DP memoizacion"
