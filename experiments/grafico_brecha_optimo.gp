set datafile separator ","
set terminal pngcairo size 1400,850 enhanced font "Arial,14"
set output "docs/results/brecha_optimo_greedy.png"
set title "Brecha de calidad de Greedy respecto al optimo de PD"
set xlabel "Cantidad de deportistas (n)"
set ylabel "Brecha respecto al optimo (%)"
set grid
set key outside right center
set logscale x
set yrange [0:*]

plot \
"build/db/team_experiment_summary.csv" using 1:(strcol(2) eq "greedy_score" ? $9 : 1/0) with linespoints title "Greedy mayor puntaje", \
"build/db/team_experiment_summary.csv" using 1:(strcol(2) eq "greedy_cost" ? $9 : 1/0) with linespoints title "Greedy menor costo", \
"build/db/team_experiment_summary.csv" using 1:(strcol(2) eq "greedy_ratio" ? $9 : 1/0) with linespoints title "Greedy ratio puntaje/costo"
