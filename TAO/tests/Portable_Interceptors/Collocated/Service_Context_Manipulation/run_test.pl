eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $status = 0;

my $process = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

my $iorbase = "test.ior";
my $iorfile = $process->LocalFile ($iorbase);
$process->DeleteFile ($iorbase);

$SV = $process->CreateProcess ("collocated");

print STDERR "======== Running in default mode \n";

$sv = $SV->SpawnWaitKill ($process->ProcessStartWaitInterval());

if ($sv != 0) {
    print STDERR "ERROR in Collocated_Test\n";
    $status = 1;
}
$process->DeleteFile ($iorbase);

print STDERR "======== Running with per-orb \n";
$SV->Arguments ("-ORBCollocation per-orb");
$sv = $SV->SpawnWaitKill ($process->ProcessStartWaitInterval());

if ($sv != 0) {
    print STDERR "ERROR in Collocated_Test\n";
    $status = 1;
}
$process->DeleteFile ($iorbase);

print STDERR "======== Running with no collocation \n";
$SV->Arguments ("-ORBCollocation no");
$sv = $SV->SpawnWaitKill ($process->ProcessStartWaitInterval());

if ($sv != 0) {
    print STDERR "ERROR in Collocated_Test\n";
    $status = 1;
}
$process->DeleteFile ($iorbase);

print STDERR "======== Running in default mode and two ORBS \n";
$SV->Arguments ("-n");
$sv = $SV->SpawnWaitKill ($process->ProcessStartWaitInterval());

if ($sv != 0) {
    print STDERR "ERROR in Collocated_Test\n";
    $status = 1;
}
$process->DeleteFile ($iorbase);

print STDERR "======== Running in per-orb mode and two ORBS \n";
$SV->Arguments ("-n -ORBCollocation per-orb");
$sv = $SV->SpawnWaitKill ($process->ProcessStartWaitInterval());

if ($sv != 0) {
    print STDERR "ERROR in Collocated_Test\n";
    $status = 1;
}
$process->DeleteFile ($iorbase);

print STDERR "======== Running in no collocation mode and two ORBS \n";
$SV->Arguments ("-n -ORBCollocation per-orb");
$sv = $SV->SpawnWaitKill ($process->ProcessStartWaitInterval());

if ($sv != 0) {
    print STDERR "ERROR in Collocated_Test\n";
    $status = 1;
}
$process->DeleteFile ($iorbase);

exit $status;
