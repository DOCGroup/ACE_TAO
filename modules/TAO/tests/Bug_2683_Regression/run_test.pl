eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$iorfile = "test.ior";

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

unlink $iorfile;
$TARGETHOSTNAME = "localhost";

if (PerlACE::is_vxworks_test()) {
    $TARGETHOSTNAME = $ENV{'ACE_RUN_VX_TGTHOST'};
    $SV = new PerlACE::ProcessVX ("server", "-ORBEndpoint iiop://$TARGETHOSTNAME:43210");
}
else {
    $SV = new PerlACE::Process ("server", "-ORBEndpoint iiop://$TARGETHOSTNAME:43210");
}

$CL = new PerlACE::Process ("client", " -p 43210 -h $TARGETHOSTNAME");

$server = $SV->Spawn ();

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    exit 1;
}

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client = $CL->SpawnWaitKill (30);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (15);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
