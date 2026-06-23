set datafile separator ","
set terminal pngcairo size 1400,850 enhanced font "Arial,14"
set output "docs/results/costo_pd_greedy.png"
set title "Costo promedio utilizado por algoritmo"
set xlabel "Cantidad de deportistas (n)"
set ylabel "Costo promedio"
set grid
set key outside right center
set logscale x
set yrange [0:3200]
# Linea de referencia del presupuesto
set arrow from graph 0, first 3000 to graph 1, first 3000 nohead dashtype 2
set label "Presupuesto = 3000" at graph 0.03, first 3050

plot \
"build/db/team_experiment_summary.csv" using 1:(strcol(2) eq "greedy_score" ? $6 : 1/0) with linespoints title "Greedy mayor puntaje", \
"build/db/team_experiment_summary.csv" using 1:(strcol(2) eq "greedy_cost" ? $6 : 1/0) with linespoints title "Greedy menor costo", \
"build/db/team_experiment_summary.csv" using 1:(strcol(2) eq "greedy_ratio" ? $6 : 1/0) with linespoints title "Greedy ratio puntaje/costo", \
"build/db/team_experiment_summary.csv" using 1:(strcol(2) eq "dp_tabulation" ? $6 : 1/0) with linespoints title "DP tabulacion", \
"build/db/team_experiment_summary.csv" using 1:(strcol(2) eq "dp_memoization" ? $6 : 1/0) with linespoints title "DP memoizacion"
