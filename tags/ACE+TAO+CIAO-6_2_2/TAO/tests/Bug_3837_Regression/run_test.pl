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

my $iorbase = "server.ior";
my $logout0 = "log.out";
my $logout1 = "log.out.1";
my $logout2 = "log.out.2";
my $logout3 = "log.out.3";
my $svcconf = "logger" . $PerlACE::svcconf_ext;
my $server_iorfile = $server->LocalFile ($iorbase);
my $server_logout0 = $server->LocalFile ($logout0);
my $server_logout1 = $server->LocalFile ($logout1);
my $server_logout2 = $server->LocalFile ($logout2);
my $server_logout3 = $server->LocalFile ($logout3);
my $server_svcconf = $server->LocalFile ($svcconf);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$server->DeleteFile($logout0);
$server->DeleteFile($logout1);
$server->DeleteFile($logout2);
$server->DeleteFile($logout3);
$client->DeleteFile($iorbase);

$tofile = "-ORBLogFile $logout0";

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $tofile = '';
    }
}

if ($server->PutFile ($svcconf) == -1) {
    print STDERR "ERROR: cannot set file <$server_svcconf>\n";
    exit 1;
}

$SV = $server->CreateProcess ("server", "-ORBdebuglevel 10 $tofile ".
                              "-ORBSvcConf $server_svcconf -o $server_iorfile ".
                              "-ORBHandleLoggingStrategyEvents Logger");
$CL = $client->CreateProcess ("client", "-k file://$client_iorfile");

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
    exit 1;
}
if ($client->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
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

if ($server->GetFile ($logout0) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_logout0>\n";
    $status = 1;
}
if ($server->GetFile ($logout1) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_logout1>\n";
    $status = 1;
}
if ($server->GetFile ($logout2) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_logout2>\n";
    $status = 1;
}
if ($server->GetFile ($logout3) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_logout3>\n";
    $status = 1;
}

if (!-e $logout0) {
    print STDERR "ERROR: log files are not rotated (missing $logout0)\n";
    $status = 1;
}
if (!-e $logout1) {
    print STDERR "ERROR: log files are not rotated (missing $logout1)\n";
    $status = 1;
}
if (!-e $logout2) {
    print STDERR "ERROR: log files are not rotated (missing $logout2)\n";
    $status = 1;
}
if (!-e $logout3) {
    print STDERR "ERROR: log files are not rotated (missing $logout3)\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$server->DeleteFile($logout0);
$server->DeleteFile($logout1);
$server->DeleteFile($logout2);
$server->DeleteFile($logout3);
$client->DeleteFile($iorbase);

exit $status;
