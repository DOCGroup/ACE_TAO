eval '(exit $?0)' && eval 'exec perl -w -S $0 ${1+"$@"}'
    & eval 'exec perl -w -S $0 $argv:q'
    if 0;

# $Id$
#
# Summarizes results from a series of runs of run_test, with
# different numbers of clients.  Example usage:
#
# $ for i in 1 2 5 10 15 20 25 30 35 40 45 50; do ./run_test $i; done
# $ ./summarize
#
# The first three lines above let this script run without specifying the
# full path to perl, as long as it is in the user's PATH.
# Taken from perlrun man page.

@files = glob 'client-*.log';
@total_threads = ();

foreach $file (@files) {
  my ($i);
  ($i = $file) =~ s/client-(\d+).log/$1/;
  push @total_threads, $i;
}

print "No.of threads\t\tAverage Latency\n\n";

foreach $total_threads (sort {$a <=> $b} @total_threads) {
  undef $high_latency;

  $high_latency = 0;
  open (FILE, "client-${total_threads}.log")  ||
    die "$0: unable to open \"client-${total_threads}.log\"\n";
  while ($line = <FILE>) {
    if ($line =~ /.*usec-per-message = ([\d\.]+)/) 
    {
        $high_latency  += $1 ;
	$number++;
    }
  }
  close FILE;

  printf "%3d\t\t\t%8f\n",
    $total_threads, $high_latency/$number;
}
