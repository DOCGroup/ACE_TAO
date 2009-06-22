eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$iorbase = "test.ior";
$iorfile = PerlACE::LocalFile ("$iorbase");

unlink $iorfile;

if (PerlACE::is_vxworks_test()) {
  $TARGETHOSTNAME = $ENV{'ACE_RUN_VX_TGTHOST'};
  $SV = new PerlACE::ProcessVX ("server", "-ORBEndpoint diop://$TARGETHOSTNAME:12345 -o $iorbase");
}
else {
  $SV = new PerlACE::Process ("server", "-ORBEndpoint diop://:12345 -o $iorfile");
}
$CL = new PerlACE::Process ("client", "-k file://$iorfile -ORBEndpoint diop://:12346");

$server = $SV->Spawn ();

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    exit 1;
}

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: could not find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (90);
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

exit $status;
