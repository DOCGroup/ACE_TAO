eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-
#
# $Id$


use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase1 = "test1.ior";
my $iorbase2 = "test2.ior";

my $server1_iorfile = $server->LocalFile ($iorbase1);
my $server2_iorfile = $server->LocalFile ($iorbase2);
my $client1_iorfile = $client->LocalFile ($iorbase1);
my $client2_iorfile = $client->LocalFile ($iorbase2);

$server->DeleteFile ($iorbase1);
$server->DeleteFile ($iorbase2);
$client->DeleteFile ($iorbase1);
$client->DeleteFile ($iorbase2);

my $SV = $server->CreateProcess ("server", "-o $server1_iorfile -o $server2_iorfile");
my $CL = $client->CreateProcess ("client", "-k file://$client1_iorfile -k file://$client2_iorfile");

$status = 0;

print STDERR "\n\n==== Running PortableInterceptor::ForwardRequest test\n";

$SV->Spawn ();

if ($server->WaitForFileTimed ($iorbase1,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server1_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($server->WaitForFileTimed ($iorbase2,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server2_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile ($iorbase1);
$server->DeleteFile ($iorbase2);
$client->DeleteFile ($iorbase1);
$client->DeleteFile ($iorbase2);

exit $status;
