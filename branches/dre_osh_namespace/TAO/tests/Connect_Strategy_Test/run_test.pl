eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("server.ior");
unlink $iorfile;
$status = 0;

$SV = new PerlACE::Process ("server", "-o $iorfile");
$CL1 = new PerlACE::Process ("client", "-k file://$iorfile");
$CL2 = new PerlACE::Process ("client", "-ORBSvcConf reactor$PerlACE::svcconf_ext -k file://$iorfile");
$CL3 = new PerlACE::Process ("client", "-ORBSvcConf blocked$PerlACE::svcconf_ext -k file://$iorfile -x 1");


$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 5) == -1) {
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

$server = $SV->WaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
