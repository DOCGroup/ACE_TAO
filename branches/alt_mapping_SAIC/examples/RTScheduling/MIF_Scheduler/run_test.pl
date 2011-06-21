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
my $nstarget = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $starter = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";

$server->AddLibPath ('..');
$client->AddLibPath ('..');
$starter->AddLibPath ('..');

my $iorbase = "ns.ior";
my $s_svcbase = "svc$PerlACE::svcconf_ext.server";
my $c_svcbase = "svc$PerlACE::svcconf_ext.client";
my $server_iorfile = $server->LocalFile ($iorbase);
my $server_svcfile = $server->LocalFile ($s_svcbase);
my $client_iorfile = $client->LocalFile ($iorbase);
my $client_svcfile = $client->LocalFile ($c_svcbase);
my $nstarget_iorfile = $nstarget->LocalFile ($iorbase);
my $starter_iorfile = $starter->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);
$nstarget->DeleteFile($iorbase);
$starter->DeleteFile($iorbase);

$NS = $nstarget->CreateProcess ("../../../orbsvcs/Naming_Service/tao_cosnaming",
                                "-o $nstarget_iorfile");
$SV = $server->CreateProcess ("test",
                              "-ORBInitRef NameService=file://$server_iorfile ".
                              "-ORBSvcConf $server_svcfile -ORBDebugLevel $debug_level");
$CL = $client->CreateProcess ("test",
                              "-ORBInitRef NameService=file://$client_iorfile ".
                              "-ORBSvcConf $client_svcfile");
$ST = $starter->CreateProcess ("Starter",
                               "-ORBInitRef NameService=file://$starter_iorfile");

print STDERR "Starting Naming Service\n";

$nstarget_status = $NS->Spawn ();

if ($nstarget_status != 0) {
    print STDERR "ERROR: ns returned $nstarget_status\n";
    exit 1;
}

if ($nstarget->WaitForFileTimed ($iorbase,
                                 $nstarget->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$nstarget_iorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

if ($nstarget->GetFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$nstarget_iorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($server->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$server_iorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($client->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($starter->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$starter_iorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

print STDERR "Starting Test 1\n";

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

sleep (10);

print STDERR "Starting Test 2\n";

$client_status = $CL->Spawn ();

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

sleep (5);

$starter_status = $ST->SpawnWaitKill ($starter->ProcessStartWaitInterval());

if ($starter_status != 0) {
    print STDERR "ERROR: starter returned $starter_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    $SV->Kill (); $SV->TimedWait (1);
    $CL->Kill (); $CL->TimedWait (1);
    exit 1;
}

$server_status = $SV->Wait ($server->ProcessStopWaitInterval() + 90);

if ($server_status == -1) {
    # if SV is still alive terminate it.
    $server_status = $SV->TerminateWaitKill ($server->ProcessStopWaitInterval());
} else {
    $server_status = $SV->Kill ($server->ProcessStopWaitInterval())
}

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$client_status = $CL->Wait ($client->ProcessStopWaitInterval());

if ($client_status == -1) {
    # if CL is still alive terminate it.
    $client_status = $CL->TerminateWaitKill ($client->ProcessStopWaitInterval());
} else {
    $client_status = $CL->Kill ($client->ProcessStopWaitInterval());
}

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$nstarget_status = $NS->TerminateWaitKill ($nstarget->ProcessStopWaitInterval());

if ($nstarget_status != 0) {
    print STDERR "ERROR: ns returned $nstarget_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);
$nstarget->DeleteFile($iorbase);
$starter->DeleteFile($iorbase);

exit $status;
