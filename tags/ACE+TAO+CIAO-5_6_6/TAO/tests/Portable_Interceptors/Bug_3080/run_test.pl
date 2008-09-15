eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-
#
# $Id$


use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$filebase = "test.ior";
$file = PerlACE::LocalFile ("$filebase");

unlink $file;

if (PerlACE::is_vxworks_test()) {
$SV1 = new PerlACE::ProcessVX ("server", "-o $filebase");
}
else {
$SV1 = new PerlACE::Process ("server", "-o $file");
}
$CL = new PerlACE::Process ("client", "-p file://$file");

$status = 0;

print STDERR "\n\n==== Running PortableInterceptor::Bug_3080 test\n";

$SV1->Spawn ();

if (PerlACE::waitforfile_timed ($file, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$file1>\n";
    $SV1->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV1->WaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $file;

exit $status;
