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

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
$server->DeleteFile($iorbase);

print STDERR "======== Running in Default Mode\n";
$SV = $server->CreateProcess ("COIOP_Test",
                              "-s -o $server_iorfile -k file://$server_iorfile " .
                              "-ORBdebuglevel $debug_level");
$server_status = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval() + 45);

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

$server->DeleteFile($iorbase);

print STDERR "======== Running with per-orb\n";
$SV->Arguments ("-s -o $server_iorfile -k file://$server_iorfile " .
                "-ORBdebuglevel $debug_level -ORBCollocation per-orb");
$server_status = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval() + 45);

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

$server->DeleteFile($iorbase);

print STDERR "======== Running with no collocation\n";
$SV->Arguments ("-o $server_iorfile -k file://$server_iorfile " .
                "-ORBdebuglevel $debug_level -ORBCollocation no");
$server_status = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval() + 45);

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

$server->DeleteFile($iorbase);

print STDERR "======== Running in default mode and two ORBS\n";
$SV->Arguments ("-s -o $server_iorfile -k file://$server_iorfile -n " .
                "-ORBdebuglevel $debug_level");
$server_status = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval() + 45);

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

$server->DeleteFile($iorbase);

print STDERR "======== Running in per-orb mode and two ORBS\n";
$SV->Arguments ("-o $server_iorfile -k file://$server_iorfile -n " .
                "-ORBdebuglevel $debug_level -ORBCollocation per-orb");
$server_status = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval() + 45);

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

$server->DeleteFile($iorbase);

print STDERR "======== Running in no collocation mode and two ORBS\n";
$SV->Arguments ("-o $server_iorfile -k file://$server_iorfile -n " .
                "-ORBdebuglevel $debug_level -ORBCollocation no");
$server_status = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval() + 45);

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

$server->DeleteFile($iorbase);

exit $status;
