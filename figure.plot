set terminal png

set output "Data/Figures/ibm01_cutstate.png"

set datafile separator ','
set title 'IBM01 Cutstate variation across FM'
set xlabel 'Timestep'
set ylabel 'Cutstate'
set style data line

plot 'Data/Results/ibm01.csv' using 1:2
