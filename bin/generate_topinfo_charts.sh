#!/bin/sh
#
# $Id$
#

gnuplot <<_EOF_ >/tmp/null.bala 2>&1
  set xdata time
  set timefmt '%Y/%m/%d-%H:%M'
  set xlabel 'Date (MM/DD)'
  set ylabel 'Size (KBytes)'
  set terminal png small color
  set yrange [3000:6000]    
  set output "$2"
  plot '$1' using 1:2 title '$3' w l
  exit
_EOF_

