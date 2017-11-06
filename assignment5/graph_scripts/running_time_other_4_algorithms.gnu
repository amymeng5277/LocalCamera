set terminal postscript eps color enh
set output 'Running time for other four algorithms'
set xlabel 'Number of vertices'
set ylabel 'Running time(us)'
set xrange[4:16]
set yrange[0:40]
set xtics 1
set ytics 2
plot "a1time.dat" using 1:2:3 with yerrorlines title 'APPROX-1',"rea1time.dat" using 1:2:3 with yerrorlines title 'REFINED-APPROX-1',"a2time.dat" using 1:2:3 with yerrorlines title 'APPROX-2',"rea2time.dat" using 1:2:3 with yerrorlines title 'REFINED-APPROX-2'
