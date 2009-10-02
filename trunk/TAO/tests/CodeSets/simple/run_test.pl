eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

PerlACE::add_lib_path ('../libs/IBM1047_ISO8859/.');
PerlACE::add_lib_path ('../libs/UCS4_UTF16/.');


my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$SV = $server->CreateProcess ("server", " -ORBDottedDecimalAddresses 1");
$CL = $client->CreateProcess ("client", " -ORBSvcConf cs_test.conf");

$status = 0;

print STDOUT "Client using char translator\n\n";

$server_status = $SV->Spawn ();
if ($server_status != 0) {
    print STDERR "ERROR: Starting server returned $server_status\n";
    exit 1;
}
if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval ());
if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval ());
if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

print STDOUT "\nServer using char translator\n\n";

$SV2 = $server->CreateProcess ("server", " -ORBDottedDecimalAddresses 1 -ORBSvcConf cs_test.conf");
$CL2 = $client->CreateProcess ("client");

$server_status = $SV2->Spawn ();
if ($server_status != 0) {
    print STDERR "ERROR: Starting server 2 returned $server_status\n";
    exit 1;
}
if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}

$client_status = $CL2->SpawnWaitKill ($client->ProcessStartWaitInterval ());
if ($client_status != 0) {
    print STDERR "ERROR: client 2 returned $client_status\n";
    $status = 1;
}

$server_status = $SV2->WaitKill ($server->ProcessStopWaitInterval ());
if ($server_status != 0) {
    print STDERR "ERROR: server 2 returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
