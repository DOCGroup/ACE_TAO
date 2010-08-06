eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-
#
# $Id$


use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$file1base = "test.ior";
$file1 = PerlACE::LocalFile ("$file1base");

unlink $file1;

if (PerlACE::is_vxworks_test()) {
$SV1 = new PerlACE::ProcessVX ("server", "-o $file1base -n 1");
}
else {
$SV1 = new PerlACE::Process ("server", "-o $file1 -n 1");
}
$CL = new PerlACE::Process ("client", "-p file://$file1");

$status = 0;

print STDERR "\n\n==== Running PortableInterceptor::Redirection test\n";

$SV1->Spawn ();

if (PerlACE::waitforfile_timed ($file1, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$file1>\n";
    $SV1->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$SV1->WaitKill (5);

print "Restarting Client to provoke TRANSIENT exception\n";

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

unlink $file1;

exit $status;
