eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$iorfile = PerlACE::LocalFile ("server.ior");

unlink $iorfile;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server");
}
else {
    $SV = new PerlACE::Process ("server");
}
$CL = new PerlACE::Process ("client", " -k file://$iorfile");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
} 

$client = $CL->SpawnWaitKill (300);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (25);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

################################
print STDERR "===== Client with RW handler\n";
if (PerlACE::is_vxworks_test()) {
    $SV1 = new PerlACE::ProcessVX ("server");
}
else {
    $SV1 = new PerlACE::Process ("server");
}
$CL1 = new PerlACE::Process ("client", 
                             " -ORBSvcConf client.conf");

$SV1->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
} 

$client1 = $CL1->SpawnWaitKill (300);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server1 = $SV1->WaitKill (25);

if ($server1 != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}


unlink $iorfile;
exit $status;
