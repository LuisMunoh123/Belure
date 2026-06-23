set datafile separator ","
set terminal pngcairo size 1400,850 enhanced font "Arial,14"
set output "docs/results/puntaje_pd_greedy.png"
set title "Puntaje promedio obtenido: PD vs Greedy"
set xlabel "Cantidad de deportistas (n)"
set ylabel "Puntaje promedio"
set grid
set key outside right center
set logscale x

plot \
"build/db/team_experiment_summary.csv" using 1:(strcol(2) eq "greedy_score" ? $7 : 1/0) with linespoints title "Greedy mayor puntaje", \
"build/db/team_experiment_summary.csv" using 1:(strcol(2) eq "greedy_cost" ? $7 : 1/0) with linespoints title "Greedy menor costo", \
"build/db/team_experiment_summary.csv" using 1:(strcol(2) eq "greedy_ratio" ? $7 : 1/0) with linespoints title "Greedy ratio puntaje/costo", \
"build/db/team_experiment_summary.csv" using 1:(strcol(2) eq "dp_tabulation" ? $7 : 1/0) with linespoints title "DP tabulacion", \
"build/db/team_experiment_summary.csv" using 1:(strcol(2) eq "dp_memoization" ? $7 : 1/0) with linespoints title "DP memoizacion"
