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
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $TARGETHOSTNAME = $server->HostName ();
my $port = $server->RandomPort ();

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$SV = $server->CreateProcess ("INS_test_server",
                              "-ORBdebuglevel $debug_level ".
                              "-ORBEndpoint iiop://1.0\@$TARGETHOSTNAME:$port ".
                              "-ORBDottedDecimalAddresses 1 ".
                              "-i object_name -o $server_iorfile");
$CL = $client->CreateProcess ("INS_test_client",
                              "random_service ".
                              "-ORBInitRef random_service=corbaloc::1.1\@".
                              "$TARGETHOSTNAME:$port/object_name");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print STDERR "\n\n==== InitRef test\n";

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print STDERR "\n\n==== InvalidName test\n";

$CL->Arguments ("not_a_service -ORBInitRef random_service=corbaloc:iiop:1.0\@".
                "$TARGETHOSTNAME:$port/object_name");
$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print STDERR "\n\n==== DefaultInitRef test\n";

$CL->Arguments ("object_name -ORBDefaultInitRef corbaloc:iiop:1.0\@".
                "$TARGETHOSTNAME:$port/");
$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print STDERR "\n\n==== Multi endpoint test\n";

$port1 = $port + 1;
$port2 = $port + 2;
$CL->Arguments ("random_service -ORBInitRef random_service=corbaloc:iiop:1.0\@".
                "$TARGETHOSTNAME:$port1,iiop:1.0\@$TARGETHOSTNAME:$port2,".
                "iiop:1.0\@$TARGETHOSTNAME:$port/object_name");
$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print STDERR "\n\n==== Multi endpoint default ref test\n";

$CL->Arguments ("object_name -ORBDefaultInitRef corbaloc:iiop:1.0\@".
                "$TARGETHOSTNAME:$port1,iiop:1.0\@$TARGETHOSTNAME:$port2,".
                "iiop:1.0\@$TARGETHOSTNAME:$port/");
$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print STDERR "\n\n==== Default ref with final '/'\n";

$CL->Arguments ("object_name -ORBDefaultInitRef corbaloc:iiop:1.0\@".
                "$TARGETHOSTNAME:$port/");
$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$server_status = $SV->TerminateWaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
