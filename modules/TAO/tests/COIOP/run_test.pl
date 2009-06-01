eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$iorbase = "test.ior";
$status = 0;

$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    } 
}

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("COIOP_Test");
    $iorfile = $iorbase;
}
else {
    $SV = new PerlACE::Process ("COIOP_Test");
    $iorfile = PerlACE::LocalFile ($iorbase);
}
unlink $iorfile;

print STDERR "======== Running in Default Mode \n";
$SV->Arguments ("-s -o $iorfile -k file://$iorfile -ORBdebuglevel $debug_level");
$sv = $SV->SpawnWaitKill (60);

if ($sv != 0) {
    print STDERR "ERROR in Collocated_Test\n";
    $status = 1;
}
unlink $iorfile;

print STDERR "======== Running with per-orb \n";
$SV->Arguments ("-s -o $iorfile -k file://$iorfile -ORBCollocation per-orb -ORBdebuglevel $debug_level");
$sv = $SV->SpawnWaitKill (60);

if ($sv != 0) {
    print STDERR "ERROR in Collocated_Test\n";
    $status = 1;
}
unlink $iorfile;

print STDERR "======== Running with no collocation \n";
$SV->Arguments ("-o $iorfile -k file://$iorfile -ORBCollocation no -ORBdebuglevel $debug_level");
$sv = $SV->SpawnWaitKill (60);

if ($sv != 0) {
    print STDERR "ERROR in Collocated_Test\n";
    $status = 1;
}
unlink $iorfile;

print STDERR "======== Running in default mode and two ORBS \n";
$SV->Arguments ("-s -o $iorfile -k file://$iorfile -n -ORBdebuglevel $debug_level");
$sv = $SV->SpawnWaitKill (60);

if ($sv != 0) {
    print STDERR "ERROR in Collocated_Test\n";
    $status = 1;
}
unlink $iorfile;

print STDERR "======== Running in per-orb mode and two ORBS \n";
$SV->Arguments ("-o $iorfile -k file://$iorfile -n -ORBCollocation per-orb -ORBdebuglevel $debug_level");
$sv = $SV->SpawnWaitKill (60);

if ($sv != 0) {
    print STDERR "ERROR in Collocated_Test\n";
    $status = 1;
}
unlink $iorfile;

print STDERR "======== Running in no collocation mode and two ORBS \n";
$SV->Arguments ("-o $iorfile -k file://$iorfile -n -ORBCollocation no -ORBdebuglevel $debug_level");
$sv = $SV->SpawnWaitKill (60);

if ($sv != 0) {
    print STDERR "ERROR in Collocated_Test\n";
    $status = 1;
}
unlink $iorfile;

exit $status;
