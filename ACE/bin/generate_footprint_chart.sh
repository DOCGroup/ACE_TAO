#!/bin/sh
#

gnuplot <<_EOF_ >/dev/null 2>&1
  set xdata time
  set xtics rotate
  set timefmt '%Y/%m/%d-%H:%M'
  set xlabel 'Date (MM/DD)'  0,-3
  set ylabel 'Size (KBytes)'
  set terminal png small size 1024,768 color
  set output "$2"
  plot '$1' using 1:(\$2/1024.0) title '$3' w l
  exit
_EOF_

