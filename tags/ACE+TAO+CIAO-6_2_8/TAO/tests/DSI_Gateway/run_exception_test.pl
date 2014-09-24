eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;


my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $gateway = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "server.ior";
my $gwbase = "gateway.ior";

my $server_iorfile = $server->LocalFile ($iorbase);
my $server_gwfile = $server->LocalFile ($gwbase);
my $gateway_iorfile = $server->LocalFile ($iorbase);
my $gateway_gwfile = $server->LocalFile ($gwbase);
my $client_gwfile = $client->LocalFile ($gwbase);

$server->DeleteFile ($iorbase);
$server->DeleteFile ($gwbase);
$gateway->DeleteFile ($iorbase);
$gateway->DeleteFile ($gwbase);
$client->DeleteFile ($gwbase);

$status = 0;

$SV = $server->CreateProcess ("server", "-o $server_iorfile");
$GW = $gateway->CreateProcess ("gateway", "-k file://$gateway_iorfile -o $gateway_gwfile");
$CL = $client->CreateProcess ("client", "-k file://$client_gwfile -u");

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

$GW->Spawn ();

if ($gateway->WaitForFileTimed ($gwbase,
                               $gateway->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$gateway_gwfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    $GW->Kill (); $GW->TimedWait (1);
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$CL = $client->CreateProcess ("client", "-k file://$client_gwfile -s");

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->Kill ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$gateway_status = $GW->Kill ();

if ($gateway_status != 0) {
    print STDERR "ERROR: gateway returned $gateway_status\n";
    $status = 1;
}

$server->DeleteFile ($iorbase);
$server->DeleteFile ($gwbase);
$gateway->DeleteFile ($iorbase);
$gateway->DeleteFile ($gwbase);
$client->DeleteFile ($gwbase);

exit $status;
