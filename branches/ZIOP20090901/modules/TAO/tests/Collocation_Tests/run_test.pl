eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target(1) || die "Create target 1 failed\n";

$iorbase = "test.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
$server->DeleteFile($iorbase);

$status = 0;

$SV = $server->CreateProcess ("Collocated_Test");

print STDERR "======== Running in Default Mode \n";
$SV->Arguments ("-o $server_iorfile -k file://$server_iorfile");
$sv = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($sv != 0) {
    print STDERR "ERROR in Collocated_Test\n";
    $status = 1;
}
$server->DeleteFile($iorbase);

print STDERR "======== Running with per-orb \n";
$SV->Arguments ("-o $server_iorfile -k file://$server_iorfile -ORBCollocation per-orb");
$sv = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($sv != 0) {
    print STDERR "ERROR in Collocated_Test\n";
    $status = 1;
}
$server->DeleteFile($iorbase);

print STDERR "======== Running with no collocation \n";
$SV->Arguments ("-o $server_iorfile -k file://$server_iorfile -ORBCollocation no");
$sv = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($sv != 0) {
    print STDERR "ERROR in Collocated_Test\n";
    $status = 1;
}
$server->DeleteFile($iorbase);

print STDERR "======== Running in default mode and two ORBS \n";
$SV->Arguments ("-o $server_iorfile -k file://$server_iorfile -n ");
$sv = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($sv != 0) {
    print STDERR "ERROR in Collocated_Test\n";
    $status = 1;
}
$server->DeleteFile($iorbase);

print STDERR "======== Running in per-orb mode and two ORBS \n";
$SV->Arguments ("-o $server_iorfile -k file://$server_iorfile -n -ORBCollocation per-orb");
$sv = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($sv != 0) {
    print STDERR "ERROR in Collocated_Test\n";
    $status = 1;
}
$server->DeleteFile($iorbase);

print STDERR "======== Running in no collocation mode and two ORBS \n";
$SV->Arguments ("-o $server_iorfile -k file://$server_iorfile -n -ORBCollocation no");
$sv = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($sv != 0) {
    print STDERR "ERROR in Collocated_Test\n";
    $status = 1;
}
$server->DeleteFile($iorbase);
$server->GetStderrLog();

exit $status;
