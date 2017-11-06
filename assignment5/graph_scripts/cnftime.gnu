set terminal postscript eps color enh
set output 'Running time for cnf'
set xlabel 'Number of vertices'
set ylabel 'Running time(us)'
set xrange[4:16]
set yrange[30:3000000]
set logscale y
set xtics 1
#set ytics 20000
plot "cnftime.dat" using 1:2:3 with yerrorlines title 'CNF-SAT'
