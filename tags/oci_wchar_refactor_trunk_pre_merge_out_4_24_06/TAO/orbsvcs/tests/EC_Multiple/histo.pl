#
# $Id$
#
# Extract a histogram, minimum, maximum and average from a file,
# filtering by a given RE.
#

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# The first three lines above let this script run without specifying the
# full path to perl, as long as it is in the user's PATH.
# Taken from perlrun man page.

use Getopt::Std;

$opt_k = 'Latency\[LCL,[A-Z]*\]';
$opt_r = 1;

getopts ('k:r:');

$max = 0;
$min = 0;
$sum = 0;
$sum2 = 0;
$n = 0;
%histo = ();

while (<>) {
    if (!m/^$opt_k/) {
	next;
    }
    chop;
    @f = split(/:/, $_);
    if ($n == 0) {
	$min = $f[1];
	$max = $f[1];
	$sum = $f[1];
	$sum2 = $f[1] * $f[1];
	$n = 1;
    } else {
	if ($min > $f[1]) {
	    $min = $f[1];
	}
	if ($max < $f[1]) {
	    $max = $f[1];
	}
	$sum += $f[1];
	$sum2 += $f[1] * $f[1];
	$n++;
    }
    $i = int ($f[1] * $opt_r);
    $histo{"$i"}++;
}

print "Latency results for $opt_k:\n";
$s2 = $sum2 / ($n - 1) - $sum / $n * $sum / ($n - 1);
if ($s2 >= 0) {
    $sigma = int(sqrt ( $s2 ));
} else {
    print "Error: $sum, $sum2, $n\n";
    $sigma = $sum2;
}

print "Min: $min,",
    " Max: $max,",
    " Avg: ", int($sum / $n),
    " Dev: ", $sigma,
    "\n";

while ( ($key,$value) = each %histo ) {
    $t = ($key / $opt_r);
    print $t, " ", 100 * $value / $n, "\n";
}

