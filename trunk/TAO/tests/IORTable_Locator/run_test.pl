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

my $server = PerlACE::TestTarget::create_target (1) || die "Create failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create failed\n";

my $TARGETHOSTNAME = $client->HostName ();
my $port = $client->RandomPort ();

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$SV = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level -ORBListenEndpoints iiop://$TARGETHOSTNAME:$port");
$CL = $client->CreateProcess ("client",
                              "-ORBdebuglevel $debug_level ".
                              "-a corbaloc:iiop:$TARGETHOSTNAME:$port/SIMPLE_TEST_KEY");

print "Starting Server\n";
$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

sleep(3);

print "Client\n";
$client_status = $CL->Spawn ();

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client_status = $CL->WaitKill ($client->ProcessStopWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
