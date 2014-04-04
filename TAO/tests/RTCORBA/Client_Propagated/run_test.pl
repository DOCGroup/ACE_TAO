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

my $iorbase = "test.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

my $svc_conf = 'svc.conf';

# copy the configuation files
if ($server->PutFile ($svc_conf) == -1) {
    print STDERR "ERROR: cannot set file <".$server->LocalFile ($svc_conf).">\n";
    exit 1;
}
if ($server->PutFile ($svc_conf.'.xml') == -1) {
    print STDERR "ERROR: cannot set file <".$server->LocalFile ($svc_conf.'.xml').">\n";
    exit 1;
}
if ($client->PutFile ($svc_conf) == -1) {
    print STDERR "ERROR: cannot set file <".$client->LocalFile ($svc_conf).">\n";
    exit 1;
}
if ($client->PutFile ($svc_conf.'.xml') == -1) {
    print STDERR "ERROR: cannot set file <".$client->LocalFile ($svc_conf.'.xml').">\n";
    exit 1;
}

print STDERR "\n            RTCORBA CLIENT_PROPAGATED Priority Unit Test\n\n";

$SV = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level -o $server_iorfile");
$CL = $client->CreateProcess ("client", "-ORBdebuglevel $debug_level -k file://$client_iorfile");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    $server = $SV->TimedWait (1);
    if ($server == 2) {
        # Mark as no longer running to avoid errors on exit.
        $SV->{RUNNING} = 0;
        exit $status;
    } else {
        print STDERR "ERROR: cannot find file <$iorbase>\n";
        $SV->Kill ();
        exit 1;
    }
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval() + 45);

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
