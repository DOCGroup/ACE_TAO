eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

$control_ior = PerlACE::LocalFile ("control.ior");
unlink $control_ior;

$proxy_ior = PerlACE::LocalFile ("proxy.ior");
unlink $proxy_ior;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("manager", "-c control.ior -p proxy.ior");
} else {
    $SV = new PerlACE::Process ("manager", "-c $control_ior -p $proxy_ior");
}
$CL = new PerlACE::Process ("client", "-c file://$control_ior -p file://$proxy_ior");

$SV->Spawn ();
if (PerlACE::waitforfile_timed ($control_ior,
                                $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$control_ior>\n";
    exit 1;
}

$client = $CL->SpawnWaitKill (10);
if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (5);
if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $control_ior;
unlink $proxy_ior;

exit $status;
