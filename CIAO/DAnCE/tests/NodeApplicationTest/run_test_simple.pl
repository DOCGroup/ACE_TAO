eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::Run_Test;

$status = 0;
$iorfile = PerlACE::LocalFile ("test.ior");

for ($iter = 0; $iter <= $#ARGV; $iter++) {
    if ($ARGV[$iter] eq "-h" || $ARGV[$iter] eq "-?") {
      print "Run_Test Perl script for NodeApplicationTest \n\n";
      print "run_test \n";
      print "\n";
      print "-h                  -- prints this information\n";
      exit 0;
  }
}


unlink $iorfile;
$CIAO_ROOT=$ENV{'CIAO_ROOT'};

$SV = new PerlACE::Process ("$CIAO_ROOT/bin/NodeApplication",
                            "-n -o $iorfile");

$CL = new PerlACE::Process ("client_simple",
                            "-k file://$iorfile ");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$CL->SpawnWaitKill (10);
$status = $SV->Kill (); $SV->TimedWait (1);

unlink $iorfile;

exit $status;
