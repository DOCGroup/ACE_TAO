eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$iorfilebase = "server.ior";
$iorfile = PerlACE::LocalFile ("$iorfilebase");
unlink $iorfile;
$status = 0;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "-o $iorfilebase");
}
else {
    $SV = new PerlACE::Process ("server", "-o $iorfile");
}
$CL1 = new PerlACE::Process ("client", "-k file://$iorfile -x");
# Use a single thread in this test, because the strategy is only for
# single threaded cases
$CL2 = new PerlACE::Process ("client", "-ORBSvcConf reactor$PerlACE::svcconf_ext -n 1 -k file://$iorfile");
$CL3 = new PerlACE::Process ("client", "-ORBSvcConf blocked$PerlACE::svcconf_ext -k file://$iorfile");


$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print STDERR "===== Base test, using LF \n";
$client1 = $CL1->SpawnWaitKill (100);

if ($client1 != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}


print STDERR "=================================== \n";
print STDERR "===== Test, using reactor to connect \n";
$client2 = $CL2->SpawnWaitKill (100);

if ($client2 != 0) {
   print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

print STDERR "========================================= \n";
print STDERR "===== Test, using blocked connect strategy\n";
$client3 = $CL3->SpawnWaitKill (100);

if ($client3 != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (15);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
