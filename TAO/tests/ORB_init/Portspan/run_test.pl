eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$port = $server->RandomPort ();

$SV1 = $server->CreateProcess ("server", "-ORBEndpoint iiop://:$port/portspan=2");
$SV2 = $server->CreateProcess ("server", "-ORBEndpoint iiop://:$port/portspan=2");
$SV3 = $server->CreateProcess ("server", "-ORBEndpoint iiop://:$port/portspan=2");

$server_status = $SV1->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

$server_status = $SV2->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $SV1->Kill ();
    exit 1;
}

$server_status = $SV3->Spawn ();
if ($server_status == 0) {
    print STDERR "ERROR: Last server didn't fail! Err:$server_status\n";
    $SV1->Kill ();
    $SV2->Kill ();
    $SV3->Kill ();
    exit 1;
}

$SV1->Kill();
$SV2->Kill();
$SV3->Kill();

print STDOUT "Portspan test completed successfully.\n";

exit $status;
