#
# $Id$
#
# Extract a histogram, minium, maximum and average for the latency
# results produced by the test.
#

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# The first three lines above let this script run without specifying the
# full path to perl, as long as it is in the user's PATH.
# Taken from perlrun man page.

use Getopt::Std;

$opt_k = 'REMOTE';
$opt_r = 1000;

getopts ('k:r:');

$max = 0;
$min = 0;
$sum = 0;
$sum2 = 0;
$n = 0;
%histo = ();

while (<>) {
    if (!m/^Latency\[$opt_k\]/) {
	next;
    }
    chop;
    @f = split(/:/, $_);
    if ($n == 0) {
	$min = $f[1];
	$max = $f[1];
	$sum = $f[1];
	$sum2 = $f[1];
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
    $i = int ($f[1] / $opt_r);
    $histo{"$i"}++;
}

print "Latency results for $opt_k:\n";
print "Minimum: $min, Maximum: $max, Average: ", int($sum / $n),
    " Deviation: ",
    int(sqrt (($sum2/$n - ($sum/$n)*($sum/$n)))), "\n";

while ( ($key,$value) = each %histo ) {
    print $key * $opt_r, " $value\n";
}

