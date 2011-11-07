eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;

$locking = "";
$nice = "";

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "if_repo.ior";

my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$SV = $server->CreateProcess ("../../../IFR_Service/tao_ifr_service",
                              " $nice " .
                              " -o $server_iorfile " .
                              " $locking");

$CL = $client->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_ifr",
                              "-ORBInitRef InterfaceRepository=file://$client_iorfile " .
                              "-T test1.idl");

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

if ($server->GetFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    $server->DeleteFile($backing_file);
    exit 1;
}
if ($client->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    $server->DeleteFile($backing_file);
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$CL->Arguments ("-ORBInitRef InterfaceRepository=file://$client_iorfile " .
                              "-T test2.idl");

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

print "Terminating IFR_Service\n";
$server_status = $SV->TerminateWaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$locking = "";

$status = 0;

$nice = "";

$ifrflags = '-Si';

$server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
$client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

$iorbase = "if_repo.ior";

$server_iorfile = $server->LocalFile ($iorbase);
$client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$SV = $server->CreateProcess ("../../../IFR_Service/tao_ifr_service",
                              " $nice " .
                              " -o $server_iorfile " .
                              " $locking");

$CL = $client->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_ifr",
                              "-ORBInitRef InterfaceRepository=file://$client_iorfile " .
                              "$ifrflags -T test1.idl");

$SV->Spawn ();

if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

foreach (1..2) {

$CL->Arguments ("-ORBInitRef InterfaceRepository=file://$client_iorfile " .
                              "$ifrflags -T test1.idl");

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$CL->Arguments ("-ORBInitRef InterfaceRepository=file://$client_iorfile " .
                              "$ifrflags -T test2.idl");

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$CL->Arguments ("-ORBInitRef InterfaceRepository=file://$client_iorfile " .
                              "$ifrflags -T test3.idl");

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}
}

print "Terminating IFR_Service\n";
$server_status = $SV->TerminateWaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
