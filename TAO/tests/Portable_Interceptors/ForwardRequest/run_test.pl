eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-
#
# $Id$


use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 1 failed\n";

my $server_iorbase = "test1.ior";
my $client_iorbase = "test2.ior";

my $server_iorfile = $server->LocalFile ($server_iorbase);
my $client_iorfile = $client->LocalFile ($client_iorbase);

$server->DeleteFile ($server_iorbase);
$client->DeleteFile ($client_iorbase);

my $SV = $server->CreateProcess ("server", "-o $server_iorfile -o $client_iorfile");


my $CL = $server->CreateProcess ("client", "-k file://$server_iorfile -k file://$client_iorfile");

$status = 0;

print STDERR "\n\n==== Running PortableInterceptor::ForwardRequest test\n";

$SV->Spawn ();

if ($server->WaitForFileTimed ($server_iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($server->WaitForFileTimed ($client_iorbase,
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

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

$server->DeleteFile ($server_iorbase);
$client->DeleteFile ($client_iorbase);

exit $status;
