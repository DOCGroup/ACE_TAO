eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$debug = "";
$which_test = "";

foreach $i (@ARGV) {
    if ($i eq "-d") {
        $debug = $i;
    }
    else {
        $which_test = $i;
    }
}

$iorfile = PerlACE::LocalFile ("test.ior");

unlink $iorfile;

if (PerlACE::is_vxworks_test()) {
  $SV = new PerlACE::ProcessVX ("server",
                              "-ORBDottedDecimalAddresses 1 "
                            . " -o test.ior");
}
else {
  $SV = new PerlACE::Process ("server",
                              "-ORBDottedDecimalAddresses 1 "
                            . " -o $iorfile");
}

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$CL = new PerlACE::Process ("client",
                            " -k file://$iorfile "
			    . " $debug "
			    . " $which_test");

$client = $CL->SpawnWaitKill (20);
$server = $SV->WaitKill (20);

unlink $iorfile;

if ($server != 0 || $client != 0) {
    exit 1;
}

exit 0;

