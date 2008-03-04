eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

$iorfile = PerlACE::LocalFile ("ior.ior");
$iogrfile = PerlACE::LocalFile ("iogr.ior");
unlink $iorfile;
unlink $iogrfile;

if (PerlACE::is_vxworks_test()) {
    $CL = new PerlACE::ProcessVX ("client");
}
else {
    $CL = new PerlACE::Process ("client");
}

$SV = new PerlACE::Process ("server");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile,
                                $PerlACE::wait_interval_for_process_creation) == -1
    || PerlACE::waitforfile_timed ($iogrfile,
                                $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client = $CL->SpawnWaitKill (300);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;
unlink $iogrfile;

exit $status;
