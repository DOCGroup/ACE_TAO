eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib '../../../../../bin';
use PerlACE::Run_Test;

print STDERR <<EOF;
INFO: Do not expect this test to work until portable interceptors
INFO: get invoked during thru_POA colocation optimizations.
INFO:
INFO: See http://deuce.doc.wustl.edu/bugzilla/show_bug.cgi?id=586
INFO: for more information.

EOF

$iorfile = PerlACE::LocalFile ("test.ior");
unlink $iorfile;
$status = 0;

$SV = new PerlACE::Process ("collocated");

print STDERR "======== Running in default mode \n";
# $SV->Arguments ("-o $iorfile -k file://$iorfile");
$sv = $SV->SpawnWaitKill (60);

if ($sv != 0) {
    print STDERR "ERROR in Collocated_Test\n";
    $status = 1;
}
unlink $iorfile;

print STDERR "======== Running with per-orb \n";
$SV->Arguments ("-ORBCollocation per-orb");
$sv = $SV->SpawnWaitKill (60);

if ($sv != 0) {
    print STDERR "ERROR in Collocated_Test\n";
    $status = 1;
}
unlink $iorfile;

print STDERR "======== Running with no collocation \n";
$SV->Arguments ("-ORBCollocation no");
$sv = $SV->SpawnWaitKill (60);

if ($sv != 0) {
    print STDERR "ERROR in Collocated_Test\n";
    $status = 1;
}
unlink $iorfile;

print STDERR "======== Running in default mode and two ORBS \n";
$SV->Arguments ("-n");
$sv = $SV->SpawnWaitKill (60);

if ($sv != 0) {
    print STDERR "ERROR in Collocated_Test\n";
    $status = 1;
}
unlink $iorfile;

print STDERR "======== Running in per-orb mode and two ORBS \n";
$SV->Arguments ("-n -ORBCollocation per-orb");
$sv = $SV->SpawnWaitKill (60);

if ($sv != 0) {
    print STDERR "ERROR in Collocated_Test\n";
    $status = 1;
}
unlink $iorfile;

print STDERR "======== Running in no collocation mode and two ORBS \n";
$SV->Arguments ("-n -ORBCollocation per-orb");
$sv = $SV->SpawnWaitKill (60);

$SV->check_return_value (0);
if ($sv != 0) {
    print STDERR "ERROR in Collocated_Test\n";
    $status = 1;
}
unlink $iorfile;

exit $status;
