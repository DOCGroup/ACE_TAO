eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../bin";
require ACEutils;

$brace="\#\#\#\#\#";
%tests = ("basic_types" => "-n 256 -l 10",
	  "tc" => "",
	  "growth" => "-l 64 -h 256 -s 4 -n 10",
	  "alignment" => "",
	  "allocator" => "");
$test = "";
$args = "";
$| = 1;
print STDERR "\n";

sub run_test
{
    #my $test = shift (@tests);
    my $args = shift (@argslist);

    print STDERR "\n$brace $test $args test BEGUN\n";
    $TST = Process::Create  ($EXEPREFIX."$test".$EXE_EXT,
                             "$args");
    print STDERR "\t$test RUNNING\n";
    $retval = $TST->TimedWait (60);
    if ($retval == -1) {
      print STDERR "ERROR $brace $test $args TIMEOUT\n";
      $TST->Kill (); $TST->TimedWait (5);
    } elsif ($retval != 0) { 
      print STDERR "ERROR $brace $test $args, exit value $retval\n";
    }
    print STDERR "$brace $test FINISHED successfully\n";
}

while (($test,$args) = each %tests)
{
    run_test ($test, $args);
}

exit 0;
