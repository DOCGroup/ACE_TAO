eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $iorbase = "test.ior";
my $iorfile = $server->LocalFile ($iorbase);

$status = 0;

$SV = $server->CreateProcess ("Collocated_Test");

$server->DeleteFile($iorbase);

print STDERR "======== Running in Default Mode \n";
$SV->Arguments ("-o $iorfile -k file://$iorfile");
$sv = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($sv != 0) {
    print STDERR "ERROR in DII Collocated twoway test\n";
    $status = 1;
}
$server->DeleteFile($iorbase);

print STDERR "======== Running with per-orb \n";
$SV->Arguments ("-o $iorfile -k file://$iorfile -ORBCollocation per-orb");
$sv = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($sv != 0) {
    print STDERR "ERROR in DII Collocated twoway test\n";
    $status = 1;
}
$server->DeleteFile($iorbase);

print STDERR "======== Running with no collocation \n";
$SV->Arguments ("-o $iorfile -k file://$iorfile -ORBCollocation no");
$sv = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($sv != 0) {
    print STDERR "ERROR in DII Collocated twoway test\n";
    $status = 1;
}
$server->DeleteFile($iorbase);

print STDERR "======== Running in default mode and two ORBS \n";
$SV->Arguments ("-o $iorfile -k file://$iorfile -n ");
$sv = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($sv != 0) {
    print STDERR "ERROR in DII Collocated twoway test\n";
    $status = 1;
}
$server->DeleteFile($iorbase);

print STDERR "======== Running in per-orb mode and two ORBS \n";
$SV->Arguments ("-o $iorfile -k file://$iorfile -n -ORBCollocation per-orb");
$sv = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($sv != 0) {
    print STDERR "ERROR in DII Collocated twoway test\n";
    $status = 1;
}
$server->DeleteFile($iorbase);

print STDERR "======== Running in no collocation mode and two ORBS \n";
$SV->Arguments ("-o $iorfile -k file://$iorfile -n -ORBCollocation no");
$sv = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($sv != 0) {
    print STDERR "ERROR in DII Collocated twoway test\n";
    $status = 1;
}
$server->DeleteFile($iorbase);

exit $status;
