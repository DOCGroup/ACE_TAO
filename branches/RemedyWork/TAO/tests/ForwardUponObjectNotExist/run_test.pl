eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client1 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $client2 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

my $iorfile = "test.ior";

#Files which used by server
my $server_iorfile = $server->LocalFile ($iorfile);
$server->DeleteFile($iorfile);

$hostname = $server->HostName ();
$port = $server->RandomPort ();

$SV = $server->CreateProcess ("server",
                              "-ORBdebuglevel $debug_level " .
                              "-ORBEndpoint iiop://$hostname:$port " .
                              "-o $server_iorfile");

# Client 1 expects OBJECT_NOT_EXIST exception ("-e 1")
$CL1 = $client1->CreateProcess ("client",
                              "-ORBdebuglevel $debug_level " .
                              "-k corbaloc::$hostname:$port/Simple_Server " .
                              "-e 1 " .
                              "-ORBForwardInvocationOnObjectNotExist 0");

# Client 2 does not expects OBJECT_NOT_EXIST exception ("-e 0")
$CL2 = $client2->CreateProcess ("client",
                              "-ORBdebuglevel $debug_level " .
                              "-x -k corbaloc::$hostname:$port/Simple_Server " .
                              "-e 0 " .
                              "-ORBForwardInvocationOnObjectNotExist 1");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($iorfile,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    KillServers ();
    exit 1;
}



print STDERR "==== Running first client with -ORBForwardInvocationOnObjectNotExist 0\n";

$client_status = $CL1->SpawnWaitKill ($client1->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

print STDERR "==== Running second client with -ORBForwardInvocationOnObjectNotExist 1\n";

$client_status = $CL2->SpawnWaitKill ($client2->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorfile);

exit $status;
