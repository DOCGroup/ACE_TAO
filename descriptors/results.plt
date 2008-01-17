set term png medium size 640, 480 xffffff x000000 x404040 xff0000 x0000ff
set xlabel "Sampling Period (10 sec)"
set ylabel "Processor Utilization"
set grid
set output "CPU.png"
set key box
set key bottom
plot "util.log" u 1 w lp t "Set-point", "util.log" u 2 w lp t "Current Utilization"


set ylabel "Execution Rate (Hz))"
set output "Rates.png"
plot "rate.log" u 1 w lp  t "Taks 1"
