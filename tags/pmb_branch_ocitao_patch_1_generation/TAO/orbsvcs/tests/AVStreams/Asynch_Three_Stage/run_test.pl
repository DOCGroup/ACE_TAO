eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../../bin';
use PerlACE::Run_Test;

# amount of delay between running the servers

$sleeptime = 2;
$status = 0;

$nsior = PerlACE::LocalFile ("ns.ior");
$testfile = PerlACE::LocalFile ("output");
$makefile = PerlACE::LocalFile ("input");

unlink $nsior;

$NS  = new PerlACE::Process ("../../../Naming_Service/Naming_Service", "-o $nsior");
$SV  = new PerlACE::Process ("sender", "-ORBInitRef NameService=file://$nsior -s sender -r 10");
$SV1  = new PerlACE::Process ("sender", "-ORBInitRef NameService=file://$nsior -s sender -r 1");
$RE1 = new PerlACE::Process ("receiver", "-ORBInitRef NameService=file://$nsior -s distributer -r receiver1 -f output1");
$RE2 = new PerlACE::Process ("receiver", "-ORBInitRef NameService=file://$nsior -s distributer -r receiver2 -f output2");
$DI  = new PerlACE::Process ("distributer", "-ORBInitRef NameService=file://$nsior -s sender -r distributer");

print STDERR "\nReceiver 1 --> Receiver 2 --> Distributer --> Sender\n\n";

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

print STDERR "Starting Distributer\n";

$DI->Spawn ();

sleep $sleeptime;

print STDERR "Starting Sender\n";

$sender = $SV->SpawnWaitKill (1000);

if ($sender != 0) {
    print STDERR "ERROR: sender returned $sender\n";
    $status = 1;
}

$distributer = $DI->TerminateWaitKill (1000);

if ($distributer != 0) {
    print STDERR "ERROR: distributer returned $distributer\n";
    $status = 1;
}

$receiver1 = $RE1->TerminateWaitKill (1000);

if ($receiver1 != 0) {
    print STDERR "ERROR: receiver returned $receiver\n";
    $status = 1;
}

$receiver2 = $RE2->TerminateWaitKill (1000);

if ($receiver2 != 0) {
    print STDERR "ERROR: receiver returned $receiver\n";
    $status = 1;
}

$nserver = $NS->TerminateWaitKill (1000);

if ($nserver != 0) {
    print STDERR "ERROR: Naming Service returned $nserver\n";
    $status = 1;
}

unlink $nsior;
unlink $testfile;

print STDERR "\nDistributer --> Receiver 1 --> Receiver 2 --> Sender\n\n";

print STDERR "Starting Naming Service\n";

$NS->Spawn ();

if (PerlACE::waitforfile_timed ($nsior, 10) == -1) {
    print STDERR "ERROR: cannot find naming service IOR file\n";
    $NS->Kill (); 
    exit 1;
}

print STDERR "Starting Distributer\n";

$DI->Spawn ();

sleep $sleeptime;

print STDERR "Starting Receiver 1\n";

$RE1->Spawn ();

sleep $sleeptime;

print STDERR "Starting Receiver 2\n";

$RE2->Spawn ();

sleep $sleeptime;

print STDERR "Starting Sender\n";

$sender = $SV->SpawnWaitKill (1000);

if ($sender != 0) {
    print STDERR "ERROR: sender returned $sender\n";
    $status = 1;
}

$distributer = $DI->TerminateWaitKill (1000);

if ($distributer != 0) {
    print STDERR "ERROR: distributer returned $distributer\n";
    $status = 1;
}

$receiver1 = $RE1->TerminateWaitKill (1000);

if ($receiver1 != 0) {
    print STDERR "ERROR: receiver returned $receiver\n";
    $status = 1;
}

$receiver2 = $RE2->TerminateWaitKill (1000);

if ($receiver2 != 0) {
    print STDERR "ERROR: receiver returned $receiver\n";
    $status = 1;
}

$nserver = $NS->TerminateWaitKill (1000);

if ($nserver != 0) {
    print STDERR "ERROR: Naming Service returned $nserver\n";
    $status = 1;
}

unlink $nsior;
unlink $testfile;

print STDERR "\nSender --> Receiver 1 --> Receiver 2 --> Distributer\n\n";

print STDERR "Starting Naming Service\n";

$NS->Spawn ();

if (PerlACE::waitforfile_timed ($nsior, 10) == -1) {
    print STDERR "ERROR: cannot find naming service IOR file\n";
    $NS->Kill (); 
    exit 1;
}

print STDERR "Starting Sender\n";

$SV1->Spawn ();

sleep $sleeptime;

print STDERR "Starting Receiver 1\n";

$RE1->Spawn ();

sleep $sleeptime;

print STDERR "Starting Receiver 2\n";

$RE2->Spawn ();

sleep $sleeptime;

print STDERR "Starting Distributer\n";

$DI->Spawn ();

$SV1->WaitKill(1000);

$distributer = $DI->TerminateWaitKill(1000);

if ($distributer != 0) {
    print STDERR "ERROR: sender returned $distributer\n";
    $status = 1;
}

$receiver1 = $RE1->TerminateWaitKill (1000);

if ($receiver1 != 0) {
    print STDERR "ERROR: receiver returned $receiver\n";
    $status = 1;
}

$receiver2 = $RE2->TerminateWaitKill (1000);

if ($receiver2 != 0) {
    print STDERR "ERROR: receiver returned $receiver\n";
    $status = 1;
}

$nserver = $NS->TerminateWaitKill (1000);

if ($nserver != 0) {
    print STDERR "ERROR: Naming Service returned $nserver\n";
    $status = 1;
}

unlink $nsior;
unlink $testfile;

exit $status;















