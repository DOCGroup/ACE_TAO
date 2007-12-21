eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-
#
# $Id$


use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$file1base = "test1.ior";
$file1 = PerlACE::LocalFile ("$file1base");
$file2 = PerlACE::LocalFile ("test2.ior");
$file3 = PerlACE::LocalFile ("test3.ior");

unlink $file1;
unlink $file2;
unlink $file3;

if (PerlACE::is_vxworks_test()) {
$SV1 = new PerlACE::ProcessVX ("server", "-o $file1base -c 1 -n 1 -s 1");
}
else {
$SV1 = new PerlACE::Process ("server", "-o $file1 -c 1 -n 1 -s 1");
}
$SV2 = new PerlACE::Process ("server", "-o $file2 -n 2 -s 2");
$SV3 = new PerlACE::Process ("server", "-o $file3 -n 3 -s 3");
$CL = new PerlACE::Process ("client", 
                    "-p file://$file1 -f file://$file2 -s file://$file3");

$status = 0;

print STDERR "\n\n==== Running PortableInterceptor::Bug_3079 test\n";

$SV1->Spawn ();
$SV2->Spawn ();
$SV3->Spawn ();

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

if (PerlACE::waitforfile_timed ($file3, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$file3>\n";
    $SV3->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server1 = $SV1->WaitKill (15);

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

$server2 = $SV2->WaitKill (15);

# The second server will crash by design and in such instance it will
# return the magic number 1. The test must not interpret it as an
# error.
if ($server2 != 0) {
    if ($server2 != 1) {
        print STDERR "ERROR: server2 returned $server2\n";
        $status = 1;
    }
    else {
        print STDERR "INFO: server2 commited suicide as expected\n";
    }
}

$server3 = $SV3->WaitKill (15);

if ($server3 != 0) {
    print STDERR "ERROR: server3 returned $server3\n";
    $status = 1;
}

unlink $file1;
unlink $file2;
unlink $file3;

exit $status;
