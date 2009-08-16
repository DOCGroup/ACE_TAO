eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs all Naming Service tests.  It starts
# all the servers and clients as necessary.

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("test.ior");
unlink $iorfile;

$debug = "";

for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") {
      print "Run_Test Perl script for TAO Param Test\n\n";
      print "run_test [-d>]\n";
      print "-d                  -- runs test in debug mode\n";
      exit 0;
    }
    elsif ($ARGV[$i] eq "-d") {
      $debug = " -d";
    }
}

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", " -ORBDottedDecimalAddresses 1");
}
else {
    $SV = new PerlACE::Process ("server", " -ORBDottedDecimalAddresses 1");
}
$CL = new PerlACE::Process ("client", "-k file://$iorfile -x $debug");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
} 

print STDERR $CL->CommandLine(), "\n";

$client = $CL->SpawnWaitKill (15);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (30);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
