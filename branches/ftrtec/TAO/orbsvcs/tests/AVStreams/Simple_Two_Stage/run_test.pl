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
$outfile = PerlACE::LocalFile ("output");

unlink $nsior;

$NS = new PerlACE::Process ("../../../Naming_Service/Naming_Service", "-o $nsior");
$SV = new PerlACE::Process ("receiver", "-ORBInitRef NameService=file://$nsior -f udp_output");
$CL = new PerlACE::Process ("sender", "-ORBInitRef NameService=file://$nsior");

print STDERR "Starting Naming Service\n";

$NS->Spawn ();

if (PerlACE::waitforfile_timed ($nsior, 10) == -1) {
    print STDERR "ERROR: cannot find naming service IOR file\n";
    $NS->Kill (); 
    exit 1;
}

print STDERR "Using UDP\n";
print STDERR "Starting Receiver\n";

$SV->Spawn ();

sleep $sleeptime;

print STDERR "Starting Sender\n";

$sender = $CL->SpawnWaitKill (200);

if ($sender != 0) {
    print STDERR "ERROR: sender returned $sender\n";
    $status = 1;
}

$receiver = $SV->TerminateWaitKill (5);

if ($receiver != 0) {
    print STDERR "ERROR: receiver returned $receiver\n";
    $status = 1;
}

$SV = new PerlACE::Process ("receiver", "-ORBInitRef NameService=file://$nsior -f tcp_output");
$CL = new PerlACE::Process ("sender", "-ORBInitRef NameService=file://$nsior -p TCP");

print STDERR "Using TCP\n";
print STDERR "Starting Receiver\n";

$SV->Spawn ();

sleep $sleeptime;

print STDERR "Starting Sender\n";

$sender = $CL->SpawnWaitKill (200);

if ($sender != 0) {
    print STDERR "ERROR: sender returned $sender\n";
    $status = 1;
}

$receiver = $SV->TerminateWaitKill (5);

if ($receiver != 0) {
    print STDERR "ERROR: receiver returned $receiver\n";
    $status = 1;
}

$SV = new PerlACE::Process ("receiver", "-ORBInitRef NameService=file://$nsior -f rtp_output");
$CL = new PerlACE::Process ("sender", "-ORBInitRef NameService=file://$nsior -p RTP/UDP");

print STDERR "Using RTP/UDP\n";
print STDERR "Starting Receiver\n";

$SV->Spawn ();

sleep $sleeptime;

print STDERR "Starting Sender\n";

$sender = $CL->SpawnWaitKill (200);

if ($sender != 0) {
    print STDERR "ERROR: sender returned $sender\n";
    $status = 1;
}

$receiver = $SV->TerminateWaitKill (5);

if ($receiver != 0) {
    print STDERR "ERROR: receiver returned $receiver\n";
    $status = 1;
}

$nserver = $NS->TerminateWaitKill (5);

if ($nserver != 0) {
    print STDERR "ERROR: Naming Service returned $nserver\n";
    $status = 1;
}

unlink $nsior;
unlink $output;

exit $status;
