#!/usr/bin/perl -w
#
# $Id$
#

foreach $t (@ARGV) {
  print "plot '", $t, "_delta.32.histo.dat'\n";
  foreach $i (2..16) {
    print "replot '", $t, "_delta.", 32*$i, ".histo.dat'\n";
  }
}
