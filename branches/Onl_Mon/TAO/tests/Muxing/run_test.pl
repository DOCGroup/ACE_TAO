eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
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
$CL1 = new PerlACE::Process ("client", " -k file://$iorfile");
$CL2 = new PerlACE::Process ("client", " -k file://$iorfile");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
} 

$CL1->Spawn (60);
$CL2->Spawn (60);

$max_wait = 60;
if ($^O == 'VMS') {
    $max_wait = 360;
}

$client1 = $CL1->WaitKill ($max_wait);

if ($client1 != 0) {
    print STDERR "ERROR: client 1 returned $client1\n";
    $status = 1;
}

$client2 = $CL2->WaitKill ($max_wait);

if ($client2 != 0) {
    print STDERR "ERROR: client 2 returned $client2\n";
    $status = 1;
}

# shutdown
$CLx = new PerlACE::Process ("client", " -k file://$iorfile -x");
$CLx->Spawn (60);
$CLx->WaitKill (15);

$server = $SV->WaitKill (15);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
