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

my $iorbase = "test.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
$server->DeleteFile($iorbase);

$SV = $server->CreateProcess ("COIOP_Naming_Test", "");

sub start_server{
    $server_status = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval() + 45);
    if ($server_status != 0) {
        print STDERR "ERROR: server returned $server_status\n";
        $status = 1;
    }
    $server->DeleteFile($iorbase);
}

print STDERR "======== Running in Default Mode \n";
$SV->Arguments ("-s -o $server_iorfile -k file://$server_iorfile -ORBdebuglevel $debug_level");
start_server ();

print STDERR "======== Running with per-orb \n";
$SV->Arguments ("-s -o $server_iorfile -k file://$server_iorfile -ORBCollocation per-orb");
start_server ();

print STDERR "======== Running with no collocation \n";
$SV->Arguments ("-o $server_iorfile -k file://$server_iorfile -ORBCollocation no");
start_server ();

print STDERR "======== Running in default mode and two ORBS \n";
$SV->Arguments ("-s -o $server_iorfile -k file://$server_iorfile -n");
start_server ();

print STDERR "======== Running in per-orb mode and two ORBS \n";
$SV->Arguments ("-o $server_iorfile -k file://$server_iorfile -n -ORBCollocation per-orb");
start_server ();

print STDERR "======== Running in no collocation mode and two ORBS \n";
$SV->Arguments ("-o $server_iorfile -k file://$server_iorfile -n -ORBCollocation no");
start_server ();

exit $status;
