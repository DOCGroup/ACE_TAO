eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../bin";
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("server.ior");

unlink $iorfile;
$status = 0;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "-o server.ior");
}
else {
    $SV = new PerlACE::Process ("server", "-o $iorfile");
}
$CL = new PerlACE::Process ("client", "-k file://$iorfile -d 10 -i 10 -x");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}


$client = $CL->SpawnWaitKill (200);

if ($client != 0) {
    $time = localtime;
    print STDERR "ERROR: client returned $client at $time\n";
    $status = 1;
}

$server = $SV->WaitKill (400);

if ($server != 0) {
    $time = localtime;
    print STDERR "ERROR: server returned $server at $time\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
