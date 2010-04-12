eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase1 = "test1.ior";
my $iorbase2 = "test2.ior";

my $server_iorfile1 = $server->LocalFile ($iorbase1);
my $client_iorfile1 = $client->LocalFile ($iorbase1);

my $server_iorfile2 = $server->LocalFile ($iorbase2);
my $client_iorfile2 = $client->LocalFile ($iorbase2);

$server->DeleteFile ($iorbase1);
$server->DeleteFile ($iorbase2);

$client->DeleteFile ($iorbase1);
$client->DeleteFile ($iorbase2);

print STDERR "\n********** RTCORBA Private Connection Unit Test\n\n";

$SV = $server->CreateProcess ("server", "-o $server_iorfile1 -p $server_iorfile2");
$CL = $client->CreateProcess ("client", "-o file://$client_iorfile1 -p file://$client_iorfile2 -ORBdebuglevel 3 ");

$SV->Spawn ();

if ($server->WaitForFileTimed ($iorbase2,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile2>\n";
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
    print STDERR "ERROR: server returned $server_status \n";
    $status = 1;
}

$server->DeleteFile ($iorbase1);
$server->DeleteFile ($iorbase2);

$client->DeleteFile ($iorbase1);
$client->DeleteFile ($iorbase2);

exit $status;
