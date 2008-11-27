eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("test.ior");
unlink $iorfile;
$status = 0;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("collocated");
}
else {
    $SV = new PerlACE::Process ("collocated");
}

print STDERR "======== Running in default mode \n";
$sv = $SV->SpawnWaitKill ($PerlACE::wait_interval_for_process_creation);

if ($sv != 0) {
    print STDERR "ERROR in Collocated_Test\n";
    $status = 1;
}
unlink $iorfile;

print STDERR "======== Running with per-orb \n";
$SV->Arguments ("-ORBCollocation per-orb");
$sv = $SV->SpawnWaitKill ($PerlACE::wait_interval_for_process_creation);

if ($sv != 0) {
    print STDERR "ERROR in Collocated_Test\n";
    $status = 1;
}
unlink $iorfile;

print STDERR "======== Running with no collocation \n";
$SV->Arguments ("-ORBCollocation no");
$sv = $SV->SpawnWaitKill ($PerlACE::wait_interval_for_process_creation);

if ($sv != 0) {
    print STDERR "ERROR in Collocated_Test\n";
    $status = 1;
}
unlink $iorfile;

print STDERR "======== Running in default mode and two ORBS \n";
$SV->Arguments ("-n");
$sv = $SV->SpawnWaitKill ($PerlACE::wait_interval_for_process_creation);

if ($sv != 0) {
    print STDERR "ERROR in Collocated_Test\n";
    $status = 1;
}
unlink $iorfile;

print STDERR "======== Running in per-orb mode and two ORBS \n";
$SV->Arguments ("-n -ORBCollocation per-orb");
$sv = $SV->SpawnWaitKill ($PerlACE::wait_interval_for_process_creation);

if ($sv != 0) {
    print STDERR "ERROR in Collocated_Test\n";
    $status = 1;
}
unlink $iorfile;

print STDERR "======== Running in no collocation mode and two ORBS \n";
$SV->Arguments ("-n -ORBCollocation per-orb");
$sv = $SV->SpawnWaitKill ($PerlACE::wait_interval_for_process_creation);

if ($sv != 0) {
    print STDERR "ERROR in Collocated_Test\n";
    $status = 1;
}
unlink $iorfile;

exit $status;
