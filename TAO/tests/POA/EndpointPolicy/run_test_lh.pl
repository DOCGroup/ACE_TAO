eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

$goodiorbase = "good.ior";
$badiorbase = "bad.ior";
$rootiorbase = "root.ior";
$goodiorfile = PerlACE::LocalFile ("$goodiorbase");
$badiorfile =  PerlACE::LocalFile ("$badiorbase");
$rootiorfile =  PerlACE::LocalFile ("$rootiorbase");
unlink $goodiorfile;
unlink $badiorfile;
unlink $rootiorfile;

$port = 12345;

if (PerlACE::is_vxworks_test()) {
    print STDERR "WARNING: The localhost variant of the endpoint test cannot run on VXWorks.";
    exit 0;
}

$sharedSV = new PerlACE::Process ("server", "-ORBDottedDecimalAddresses 0 -ORBUseSharedProfile 1 -h local  -g $goodiorfile -b $badiorfile -p $port");

$multiSV = new PerlACE::Process ("server", "-ORBDottedDecimalAddresses 0 -ORBUseSharedProfile 0 -h local -g $goodiorfile -b $badiorfile -p $port");


$goodCL = new PerlACE::Process ("client", " -k file://$goodiorfile");
$badCL = new PerlACE::Process ("client", " -b -k file://$badiorfile");

print "Starting server using shared profiles\n";

$sharedSV->Spawn ();

if (PerlACE::waitforfile_timed ($goodiorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$goodiorfile>\n";
    $sharedSV->Kill (); $sharedSV->TimedWait (1);
    exit 1;
}

$client = $badCL->SpawnWaitKill (300);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$client = $goodCL->SpawnWaitKill (300);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $sharedSV->WaitKill (15);

if ($server != 0) {
    print STDERR "ERROR: server [single profile per IOR] returned $server\n";
    $status = 1;
}

unlink $goodiorfile;
unlink $badiorfile;

print "Starting server using multiple profiles\n";

$multiSV->Spawn();

if (PerlACE::waitforfile_timed ($goodiorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$goodiorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client = $badCL->SpawnWaitKill (300);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$client = $goodCL->SpawnWaitKill (300);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $multiSV->WaitKill (15);

if ($server != 0) {
    print STDERR "ERROR: server [multiple profiles per IOR] returned $server\n";
    $status = 1;
}

unlink $goodiorfile;
unlink $badiorfile;
unlink $rootiorfile;

exit $status;
