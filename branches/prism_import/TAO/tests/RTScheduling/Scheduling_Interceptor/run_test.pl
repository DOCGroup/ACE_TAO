eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

# amount of delay between running the servers

$sleeptime = 2;
$status = 0;
$iorfile = PerlACE::LocalFile("test.ior");

unlink $iorfile;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("Scheduler_Interceptor_Server", "-f test.ior");
}
else {
    $SV = new PerlACE::Process ("Scheduler_Interceptor_Server", "-f test.ior");
}
$CL = new PerlACE::Process ("Scheduler_Interceptor_Client", "-f $iorfile");

print STDERR "Starting Server\n";

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

print STDERR "Starting Client\n";

$sender = $CL->SpawnWaitKill (200);

if ($sender != 0) {
    print STDERR "ERROR: sender returned $sender\n";
    $status = 1;
}

$receiver = $SV->TerminateWaitKill (5);

if ($receiver != 0) {
    print STDERR "ERROR: receiver returned $receiver\n";
    $status = 1;
}

exit $status;
