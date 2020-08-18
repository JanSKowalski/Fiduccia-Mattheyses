set terminal png

set output "pins.png"
set datafile separator ','
#set style data line

#set title 'Efficiency'
#set xlabel 'Number of pins cells'
#set ylabel 'Time in seconds'


#file_name(n) = sprintf("file_%d.dat", n)
#plot for[i = 1:2] file_name_single(i) title file_name_single(i)
#plot for[i = 10:18] file_name_double(i) title file_name_double(i)

f(x) = m*x + b
fit f(x) 'pins.csv' using 1:2 via m,b

plot 'pins.csv' using 1:2, f(x)
