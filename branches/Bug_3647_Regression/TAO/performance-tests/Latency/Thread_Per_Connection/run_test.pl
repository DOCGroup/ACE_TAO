eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$iorfile = PerlACE::LocalFile ("test.ior");
$iteration = 250000;
unlink $iorfile;



for ($iter = 0; $iter <= $#ARGV; $iter++) {
    if ($ARGV[$iter] eq "-h" || $ARGV[$iter] eq "-?") {
      print "Run_Test Perl script for Thread-per-Connection Latency test\n\n";
      print "run_test [-n num] [-h] \n";
      print "\n";
      print "-n num              -- runs the client num times\n";
      print "-h                  -- prints this information\n";
      exit 0;
  }
  elsif ($ARGV[$iter] eq "-n") {
      $iteration = $ARGV[$iter + 1];
      $i++;
  }
    
}

print STDERR "================ Thread-per-Connection Latency Test\n";

$SV = new PerlACE::Process ("server",
                            "-o $iorfile");

$CL = new PerlACE::Process ("client",
                            "-k file://$iorfile "
                            . " -i $iteration");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (120);
$server = $SV->WaitKill (20);

unlink $iorfile;

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

exit $status;
