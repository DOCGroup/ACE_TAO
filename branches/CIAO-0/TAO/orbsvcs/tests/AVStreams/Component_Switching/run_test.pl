eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../../bin';
use PerlACE::Run_Test;

# amount of delay between running the servers

$sleeptime = 2;
$distributor_time = 5;
$sender_time = 8;
$status = 0;

$nsior = PerlACE::LocalFile ("ns.ior");
$testfile = PerlACE::LocalFile ("output");
$makefile = PerlACE::LocalFile ("input");

unlink $nsior;

$NS  = new PerlACE::Process ("../../../Naming_Service/Naming_Service", "-o $nsior");
$SV1  = new PerlACE::Process ("sender", "-ORBSvcConf components_svc.conf -ORBInitRef NameService=file://$nsior -s sender -r 30");
$SV2  = new PerlACE::Process ("sender", "-ORBSvcConf components_svc.conf -ORBInitRef NameService=file://$nsior -s sender -r 30");
$SV3  = new PerlACE::Process ("sender", "-ORBSvcConf components_svc.conf -ORBInitRef NameService=file://$nsior -s sender -r 30");
$RE1 = new PerlACE::Process ("receiver", "-ORBSvcConf components_svc.conf -ORBInitRef NameService=file://$nsior -s distributer -r receiver1 -f output1");
$RE2 = new PerlACE::Process ("receiver", "-ORBSvcConf components_svc.conf -ORBInitRef NameService=file://$nsior -s distributer -r receiver2 -f output2");
$DI1 = new PerlACE::Process ("distributer", "-ORBSvcConf components_svc.conf -ORBInitRef NameService=file://$nsior -s sender -r distributer");
$DI2 = new PerlACE::Process ("distributer", "-ORBSvcConf components_svc.conf -ORBInitRef NameService=file://$nsior -s sender -r distributer");
$DI3 = new PerlACE::Process ("distributer", "-ORBSvcConf components_svc.conf -ORBInitRef NameService=file://$nsior -s sender -r distributer");
$DI4 = new PerlACE::Process ("distributer", "-ORBSvcConf components_svc.conf -ORBInitRef NameService=file://$nsior -s sender -r distributer");

print STDERR "\nReceiver 1 --> Receiver 2 --> Distributer 1 --> Sender1 --> Distributer 2 --> Distributer 3 --> Sender2 --> Sender3 --> Distributer4\n\n";

print STDERR "Starting Naming Service\n";

$NS->Spawn ();

if (PerlACE::waitforfile_timed ($nsior, 10) == -1) {
    print STDERR "ERROR: cannot find naming service IOR file\n";
    $NS->Kill (); 
    exit 1;
}

print STDERR "Starting Receiver 1\n";

$RE1->Spawn ();

sleep $sleeptime;

print STDERR "Starting Receiver 2\n";

$RE2->Spawn ();

sleep $sleeptime;

print STDERR "Starting Distributer 1\n";

$DI1->Spawn ();

sleep $sleeptime;

print STDERR "Starting Sender1\n";

$SV1->Spawn ();

sleep $distributor_time;

print STDERR "\nStarting Distributer 2\n\n";

$DI2->Spawn ();

sleep $distributor_time;

print STDERR "\nStarting Distributer 3\n\n";

$DI3->Spawn ();

sleep $sender_time;

print STDERR "Starting Sender2\n";

$SV2->Spawn ();

sleep $sender_time;

print STDERR "Starting Sender3\n";

$SV3->Spawn ();

sleep $distributer_time;

print STDERR "\nStarting Distributer 4\n\n";

$distributer4 = $DI4->SpawnWaitKill (1500);

if ($distributer4 != 0) {
    print STDERR "ERROR: distributer4 returned $distributer4\n";
    $status = 1;
}

$sender2 = $SV2->TerminateWaitKill (5);

if ($sender2 != 0) {
    print STDERR "ERROR: sender2 returned $sender2\n";
    $status = 1;
}

$sender3 = $SV3->TerminateWaitKill (5);

if ($sender3 != 0) {
    print STDERR "ERROR: sender3 returned $sender3\n";
    $status = 1;
}

$distributer3 = $DI3->TerminateWaitKill (5);

if ($distributer3 != 0) {
    print STDERR "ERROR: distributer3 returned $distributer3\n";
    $status = 1;
}

$distributer2 = $DI2->TerminateWaitKill (5);

if ($distributer2 != 0) {
    print STDERR "ERROR: distributer2 returned $distributer2\n";
    $status = 1;
}

$distributer1 = $DI1->TerminateWaitKill (5);

if ($distributer1 != 0) {
    print STDERR "ERROR: distributer1 returned $distributer1\n";
    $status = 1;
}

$receiver1 = $RE1->TerminateWaitKill (5);

if ($receiver1 != 0) {
    print STDERR "ERROR: receiver1 returned $receiver1\n";
    $status = 1;
}

$receiver2 = $RE2->TerminateWaitKill (5);

if ($receiver2 != 0) {
    print STDERR "ERROR: receiver2 returned $receiver2\n";
    $status = 1;
}

$sender = $SV1->TerminateWaitKill (5);

if ($sender != 0) {
    print STDERR "ERROR: sender returned $sender\n";
    $status = 1;
}

$nserver = $NS->TerminateWaitKill (5);

if ($nserver != 0) {
    print STDERR "ERROR: Naming Service returned $nserver\n";
    $status = 1;
}

unlink $nsior;
unlink $testfile;

exit $status;



