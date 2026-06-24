set output "docs/results/tiempo_pd_greedy_lineas.png"

set title "Tiempo promedio de ejecucion: PD vs Greedy"
set xlabel "Cantidad de deportistas (n)"
set ylabel "Tiempo promedio (microsegundos)"

set grid
set key outside right center
set logscale x
set xrange [10:2000]
set yrange [0:*]

plot \
"< awk -F',' 'NR > 1 && $2 == \"greedy_score\" {print $1, $5}' build/db/team_experiment_summary.csv" \
using 1:2 with linespoints lw 3 pt 7 ps 1.2 title "Greedy mayor puntaje", \
"< awk -F',' 'NR > 1 && $2 == \"greedy_cost\" {print $1, $5}' build/db/team_experiment_summary.csv" \
using 1:2 with linespoints lw 3 pt 5 ps 1.2 title "Greedy menor costo", \
"< awk -F',' 'NR > 1 && $2 == \"greedy_ratio\" {print $1, $5}' build/db/team_experiment_summary.csv" \
using 1:2 with linespoints lw 3 pt 9 ps 1.2 title "Greedy ratio puntaje/costo", \
"< awk -F',' 'NR > 1 && $2 == \"dp_tabulation\" {print $1, $5}' build/db/team_experiment_summary.csv" \
using 1:2 with linespoints lw 3 pt 11 ps 1.2 title "DP tabulacion", \
"< awk -F',' 'NR > 1 && $2 == \"dp_memoization\" {print $1, $5}' build/db/team_experiment_summary.csv" \
using 1:2 with linespoints lw 3 pt 13 ps 1.2 title "DP memoizacionset datafile separator ","
set terminal pngcairo size 1400,850 font "Arial,14"

# =========================================================
# GRAFICO 1: PUNTAJE PROMEDIO
# =========================================================
set output "docs/results/puntaje_pd_greedy.png"
set title "Puntaje promedio obtenido: PD vs Greedy"
set xlabel "Cantidad de deportistas (n)"
set ylabel "Puntaje promedio"
set grid
set key outside right center
set logscale x
set xrange [10:2000]
unset logscale y
set yrange [0:*]

plot \
"< awk -F',' 'NR>1 && $2==\"greedy_score\" {print $1, $7}' build/db/team_experiment_summary.csv" using 1:2 with linespoints lw 3 pt 7 ps 1.2 title "Greedy mayor puntaje", \
"< awk -F',' 'NR>1 && $2==\"greedy_cost\" {print $1, $7}' build/db/team_experiment_summary.csv" using 1:2 with linespoints lw 3 pt 5 ps 1.2 title "Greedy menor costo", \
"< awk -F',' 'NR>1 && $2==\"greedy_ratio\" {print $1, $7}' build/db/team_experiment_summary.csv" using 1:2 with linespoints lw 3 pt 9 ps 1.2 title "Greedy ratio puntaje/costo", \
"< awk -F',' 'NR>1 && $2==\"dp_tabulation\" {print $1, $7}' build/db/team_experiment_summary.csv" using 1:2 with linespoints lw 3 pt 11 ps 1.2 title "DP tabulacion", \
"< awk -F',' 'NR>1 && $2==\"dp_memoization\" {print $1, $7}' build/db/team_experiment_summary.csv" using 1:2 with linespoints lw 3 pt 13 ps 1.2 title "DP memoizacion"

unset output


# =========================================================
# GRAFICO 2: BRECHA RESPECTO AL OPTIMO
# =========================================================
set output "docs/results/brecha_optimo_greedy.png"
set title "Brecha de calidad de Greedy respecto al optimo de PD"
set xlabel "Cantidad de deportistas (n)"
set ylabel "Brecha respecto al optimo (%)"
set grid
set key outside right center
set logscale x
set xrange [10:2000]
unset logscale y
set yrange [0:*]

plot \
"< awk -F',' 'NR>1 && $2==\"greedy_score\" {print $1, $9}' build/db/team_experiment_summary.csv" using 1:2 with linespoints lw 3 pt 7 ps 1.2 title "Greedy mayor puntaje", \
"< awk -F',' 'NR>1 && $2==\"greedy_cost\" {print $1, $9}' build/db/team_experiment_summary.csv" using 1:2 with linespoints lw 3 pt 5 ps 1.2 title "Greedy menor costo", \
"< awk -F',' 'NR>1 && $2==\"greedy_ratio\" {print $1, $9}' build/db/team_experiment_summary.csv" using 1:2 with linespoints lw 3 pt 9 ps 1.2 title "Greedy ratio puntaje/costo"

unset output


# =========================================================
# GRAFICO 3: TIEMPO PROMEDIO
# =========================================================
set output "docs/results/tiempo_pd_greedy.png"
set title "Tiempo promedio de ejecucion: PD vs Greedy"
set xlabel "Cantidad de deportistas (n)"
set ylabel "Tiempo promedio (microsegundos)"
set grid
set key outside right center
set logscale x
set xrange [10:2000]
set logscale y
set yrange [0.1:*]

plot \
"< awk -F',' 'NR>1 && $2==\"greedy_score\" && $5>0 {print $1, $5}' build/db/team_experiment_summary.csv" using 1:2 with linespoints lw 3 pt 7 ps 1.2 title "Greedy mayor puntaje", \
"< awk -F',' 'NR>1 && $2==\"greedy_cost\" && $5>0 {print $1, $5}' build/db/team_experiment_summary.csv" using 1:2 with linespoints lw 3 pt 5 ps 1.2 title "Greedy menor costo", \
"< awk -F',' 'NR>1 && $2==\"greedy_ratio\" && $5>0 {print $1, $5}' build/db/team_experiment_summary.csv" using 1:2 with linespoints lw 3 pt 9 ps 1.2 title "Greedy ratio puntaje/costo", \
"< awk -F',' 'NR>1 && $2==\"dp_tabulation\" && $5>0 {print $1, $5}' build/db/team_experiment_summary.csv" using 1:2 with linespoints lw 3 pt 11 ps 1.2 title "DP tabulacion", \
"< awk -F',' 'NR>1 && $2==\"dp_memoization\" && $5>0 {print $1, $5}' build/db/team_experiment_summary.csv" using 1:2 with linespoints lw 3 pt 13 ps 1.2 title "DP memoizacion"

unset output


# =========================================================
# GRAFICO 4: PRESUPUESTO NO UTILIZADO
# =========================================================
set output "docs/results/presupuesto_no_utilizado.png"
set title "Presupuesto no utilizado por algoritmo"
set xlabel "Cantidad de deportistas (n)"
set ylabel "Presupuesto no utilizado"
set grid
set key outside right center
set logscale x
set xrange [10:2000]
unset logscale y
set yrange [0:*]

plot \
"< awk -F',' 'NR>1 && $2==\"greedy_score\" {print $1, 3000-$6}' build/db/team_experiment_summary.csv" using 1:2 with linespoints lw 3 pt 7 ps 1.2 title "Greedy mayor puntaje", \
"< awk -F',' 'NR>1 && $2==\"greedy_cost\" {print $1, 3000-$6}' build/db/team_experiment_summary.csv" using 1:2 with linespoints lw 3 pt 5 ps 1.2 title "Greedy menor costo", \
"< awk -F',' 'NR>1 && $2==\"greedy_ratio\" {print $1, 3000-$6}' build/db/team_experiment_summary.csv" using 1:2 with linespoints lw 3 pt 9 ps 1.2 title "Greedy ratio puntaje/costo", \
"< awk -F',' 'NR>1 && $2==\"dp_tabulation\" {print $1, 3000-$6}' build/db/team_experiment_summary.csv" using 1:2 with linespoints lw 3 pt 11 ps 1.2 title "DP tabulacion", \
"< awk -F',' 'NR>1 && $2==\"dp_memoization\" {print $1, 3000-$6}' build/db/team_experiment_summary.csv" using 1:2 with linespoints lw 3 pt 13 ps 1.2 title "DP memoizacion"

unset output


# =========================================================
# GRAFICO 5: LIMITES PRACTICOS
# =========================================================
set output "docs/results/limites_recursos.png"
set title "Limite practico de ejecucion por algoritmo"
set xlabel "Cantidad de deportistas (n)"
set ylabel "Tiempo de ejecucion (microsegundos)"
set grid
set key outside right center
set logscale x
set xrange [10:2000]
set logscale y
set yrange [1:*]

plot \
"< awk -F',' 'NR>1 && $2==\"greedy_ratio\" && $3>0 && $4==1 {print $1, $3}' build/db/team_resource_limits.csv" using 1:2 with linespoints lw 3 pt 7 ps 1.2 title "Greedy ratio", \
"< awk -F',' 'NR>1 && $2==\"dp_tabulation\" && $3>0 && $4==1 {print $1, $3}' build/db/team_resource_limits.csv" using 1:2 with linespoints lw 3 pt 5 ps 1.2 title "DP tabulacion", \
"< awk -F',' 'NR>1 && $2==\"dp_memoization\" && $3>0 && $4==1 {print $1, $3}' build/db/team_resource_limits.csv" using 1:2 with linespoints lw 3 pt 9 ps 1.2 title "DP memoizacion"

unset output"
