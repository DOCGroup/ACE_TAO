eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-
#
# $Id$


use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$file1 = PerlACE::LocalFile ("test1.ior");
$file2 = PerlACE::LocalFile ("test2.ior");

unlink $file1;
unlink $file2;

if (PerlACE::is_vxworks_test()) {
$SV1 = new PerlACE::ProcessVX ("server", "-o test1.ior -c 1 -n 1");
}
else {
$SV1 = new PerlACE::Process ("server", "-o $file1 -c 1 -n 1");
}
$SV2 = new PerlACE::Process ("server", "-o $file2 -n 2");
$CL = new PerlACE::Process ("client", "-p file://$file1 -s file://$file2");

$status = 0;

print STDERR "\n\n==== Running PortableInterceptor::Redirection test\n";

$SV1->Spawn ();
$SV2->Spawn ();

if (PerlACE::waitforfile_timed ($file1, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$file1>\n";
    $SV1->Kill ();
    exit 1;
}

if (PerlACE::waitforfile_timed ($file2, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$file2>\n";
    $SV2->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server1 = $SV1->WaitKill (5);

# The first server will crash by design and in such instance it will
# return the magic number 1. The test must not interpret it as an
# error.

if ($server1 != 0) {
    if ($server1 != 1) {
        print STDERR "ERROR: server1 returned $server1\n";
        $status = 1;
    }
    else {
        print STDERR "INFO: server1 commited suicide as expected\n";
    }
}

$server2 = $SV2->WaitKill (5);

if ($server2 != 0) {
    print STDERR "ERROR: server2 returned $server2\n";
    $status = 1;
}

unlink $file1;
unlink $file2;

exit $status;
