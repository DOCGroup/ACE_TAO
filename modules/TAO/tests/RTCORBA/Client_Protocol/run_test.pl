eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

$status = 0;

my $iorbase1 = "test1.ior";
my $iorbase2 = "test2.ior";

my $server_iorfile1 = $server->LocalFile ($iorbase1);
my $server_iorfile2 = $server->LocalFile ($iorbase2);

my $client_iorfile1 = $client->LocalFile ($iorbase1);
my $client_iorfile2 = $client->LocalFile ($iorbase2);

$server->DeleteFile ($iorbase1);
$server->DeleteFile ($iorbase2);

$client->DeleteFile ($iorbase1);
$client->DeleteFile ($iorbase2);

print STDERR "\n********** RTCORBA Client Protocol Policy Unit Test\n\n";

# Arguments are platform-dependent (UIOP not available on Windows).
$server_args =

    (($^O eq "MSWin32") ? "-p 1413566210 " : "-p 1413566208 ")
    ."-ORBendpoint iiop:// -ORBendpoint shmiop:// "
    .(($^O eq "MSWin32" || $^O eq "VMS") ? "" : "-ORBEndpoint uiop:// ");


$client_args =
    "-s file://$client_iorfile1 -c file://$client_iorfile2 "
    .(($^O eq "MSWin32" || $^O eq "VMS") ? "" : "-p 1413566210 ")
    ."-ORBdebuglevel 1 ";

# Start server.
$SV = $server->CreateProcess ("server", "-s $server_iorfile1 -c $server_iorfile2 $server_args");

$CL = $client->CreateProcess ("client", $client_args);

$SV->Spawn ();

if ($server->WaitForFileTimed ($iorbase2,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile2>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

# Start client.
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

$server->DeleteFile ($iorbase1);
$server->DeleteFile ($iorbase2);

$client->DeleteFile ($iorbase1);
$client->DeleteFile ($iorbase2);

# Clean up SHMIOP files
PerlACE::check_n_cleanup_files ("server_shmiop_*");

exit $status;
