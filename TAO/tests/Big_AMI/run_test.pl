eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$debug_level = '0';
$iterations = '10';
$payload = '256000';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '1';
    }
}

$iorfile = PerlACE::LocalFile ("server.ior");

unlink $iorfile;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "-ORBdebuglevel $debug_level -d -o server.ior");
}
else {
    $SV = new PerlACE::Process ("server", "-ORBdebuglevel $debug_level -d -o $iorfile");
}

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$CL = new PerlACE::Process ("client", " -k file://$iorfile -i $iterations -x -b $payload");

$client = $CL->SpawnWaitKill (60);
$server = $SV->WaitKill (10);

unlink $iorfile;

if ($server != 0 || $client != 0) {
    exit 1;
}

exit 0;

