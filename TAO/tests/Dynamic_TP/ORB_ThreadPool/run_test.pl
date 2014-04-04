eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$threads = 30;
$delay = 2;

$sdebug = "";
$cdebug = "";
$mdebug = "";
foreach $i (@ARGV) {
    if ($i eq '-sdebug' || $i eq '-debug') {
        $sdebug = "-ORBDebuglevel 10 -ORBVerboseLogging 1";
    }
    if ($i eq '-mdebug' || $i eq '-debug') {
        $mdebug = "-ORBDebuglevel 10 -ORBVerboseLogging 1";
    }
    if ($i eq '-cdebug' || $i eq '-debug') {
        $sdebug = "-ORBDebuglevel 10 -ORBVerboseLogging 1";
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $middle = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $siorbase = "server.ior";
my $miorbase = "middle.ior";
my $server_iorfile = $server->LocalFile ($siorbase);
my $middle_ciorfile = $middle->LocalFile ($siorbase);
my $middle_siorfile = $middle->LocalFile ($miorbase);
my $client_iorfile = $client->LocalFile ($miorbase);
$server->DeleteFile($siorbase);
$middle->DeleteFile($siorbase);
$middle->DeleteFile($miorbase);
$client->DeleteFile($miorbase);

# copy the configuation files
if ($server->PutFile ('server.conf') == -1) {
    print STDERR "ERROR: cannot set file <".$server->LocalFile ('server.conf').">\n";
    exit 1;
}
if ($middle->PutFile ('middle.conf') == -1) {
    print STDERR "ERROR: cannot set file <".$middle->LocalFile ('middle.conf').">\n";
    exit 1;
}
if ($client->PutFile ('client.conf') == -1) {
    print STDERR "ERROR: cannot set file <".$client->LocalFile ('client.conf').">\n";
    exit 1;
}

$SV = $server->CreateProcess ("server", "$sdebug -ORBSvcConf server.conf -ORBDynamicThreadPoolName ORBPool -o $server_iorfile");
$MD = $middle->CreateProcess ("middle", "$mdebug -ORBSvcConf middle.conf -ORBDynamicThreadPoolName ORBPool -o $middle_siorfile -k file://$middle_ciorfile");
$CL = $client->CreateProcess ("client", "$cdebug -ORBSvcConf client.conf -k file://$client_iorfile -n $threads -s $delay -x");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($siorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($server->GetFile ($siorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$middle_status = $MD->Spawn ();

if ($middle_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($middle->WaitForFileTimed ($miorbase,
                               $middle->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $MD->Kill (); $MD->TimedWait (1);
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($middle->GetFile ($miorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$middle_siorfile>\n";
    $MD->Kill (); $MD->TimedWait (1);
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($client->PutFile ($miorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 10);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$middle_status = $MD->WaitKill ($middle->ProcessStopWaitInterval());

if ($middle_status != 0) {
    print STDERR "ERROR: middle returned $middle_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($siorbase);
$middle->DeleteFile($siorbase);
$middle->DeleteFile($miorbase);
$client->DeleteFile($miorbase);

exit $status;
