eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 1 failed\n";

my $iorfile = "test.ior";
my $server_iorfile = $server->LocalFile ($iorfile);
my $client_iorfile = $client->LocalFile ($iorfile);
$server->DeleteFile($iorfile);
$client->DeleteFile($iorfile);


$SV = $server->CreateProcess ("server", "-o $server_iorfile");
$CL = $client->CreateProcess ("client", "-f file://$client_iorfile");

print STDERR "\n\n==== Running interceptor test\n";

$SV->Spawn ();

if ($server->WaitForFileTimed ($iorfile,
                                $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStartWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

$server->DeleteFile($iorfile);
$client->DeleteFile($iorfile);

exit $status;
