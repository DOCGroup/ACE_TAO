eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

$iorbase = "obv.ior";
$iorfile = PerlACE::LocalFile ("$iorbase");

unlink $iorfile;

if (PerlACE::is_vxworks_test()) {
  $SV = new PerlACE::ProcessVX ("server", "-o $iorbase");
}
else {
  $SV = new PerlACE::Process ("server", "-o $iorfile");
}
$CL = new PerlACE::Process ("client", "-f $iorfile");

$server = $SV->Spawn ();

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    exit 1;
}

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: timed out waiting for file <$iorfile>\n";
    $SV->Kill (); 
    exit 1;
}

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: the client returned $client\n";
    $status = 1;
}

$server = $SV->TerminateWaitKill (15); 

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
