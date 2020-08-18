set terminal png

set output "Data/Figures/ibm00_cutstate.png"
set datafile separator ','
set style data line

#set title 'Cutstate variation across FM'
#set xlabel 'Timestep'
#set ylabel 'Cutstate'


#file_name(n) = sprintf("file_%d.dat", n)
#plot for[i = 1:2] file_name_single(i) title file_name_single(i)
#plot for[i = 10:18] file_name_double(i) title file_name_double(i)


plot 'Data/Results/ibm00.csv' using 1:2
