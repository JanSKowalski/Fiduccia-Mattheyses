#Figure 1, linear results
set terminal png

set output "mutation_frequency.png"
set datafile separator ','
#set style data line


set key off

#set format x "%.0s^%T"
#set xrange [0.6:0.72]
#set xtics 10000,25000,240000
#set xtics rotate by 40 right

set title 'The cutstates produced by GA as mutation freq. varies'
#set xlabel 'Mutation frequency value'
#set ylabel 'Cutstate value (lower is better)'

set ticslevel 0
set dgrid3d 30,30
set hidden3d
splot 'results.csv' using 1:2:3 with lines
