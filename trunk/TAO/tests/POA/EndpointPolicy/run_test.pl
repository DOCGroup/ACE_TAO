eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

$status = 0;

$goodiorfile = PerlACE::LocalFile ("good.ior");
$badiorfile =  PerlACE::LocalFile ("bad.ior");
unlink $goodiorfile;
unlink $badiorfile;

$port = 12345;

if (PerlACE::is_vxworks_test()) {
    $sharedSV = new PerlACE::ProcessVX ("server", "-ORBDottedDecimalAddresses 0 -ORBUseSharedProfile 1 -g $goodiorfile -b $badiorfile -p $port");
}
else {
    $sharedSV = new PerlACE::Process ("server", "-ORBDottedDecimalAddresses 0 -ORBUseSharedProfile 1 -g $goodiorfile -b $badiorfile -p $port");
}

if (PerlACE::is_vxworks_test()) {
    $multiSV = new PerlACE::ProcessVX ("server", "-ORBDottedDecimalAddresses 0 -ORBUseSharedProfile 0 -g $goodiorfile -b $badiorfile -p $port");
}
else {
    $multiSV = new PerlACE::Process ("server", "-ORBDottedDecimalAddresses 0 -ORBUseSharedProfile 0 -g $goodiorfile -b $badiorfile -p $port");
}

$goodCL = new PerlACE::Process ("client", " -k file://$goodiorfile");
$badCL = new PerlACE::Process ("client", " -b -k file://$badiorfile");

print "Starting server using shared profiles\n";

$sharedSV->Spawn ();

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

$server = $sharedSV->WaitKill (10);

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

$server = $multiSV->WaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: server [multiple profiles per IOR] returned $server\n";
    $status = 1;
}

unlink $goodiorfile;
unlink $badiorfile;

exit $status;
