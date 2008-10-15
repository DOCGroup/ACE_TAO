eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$iorbase = "test.ior";

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("Collocated_Test");
    $iorfile = $iorbase;
}
else {
    $SV = new PerlACE::Process ("Collocated_Test");
    $iorfile = PerlACE::LocalFile ($iorbase);
}
unlink $iorfile;

print STDERR "======== Running in Default Mode \n";
$SV->Arguments ("-o $iorfile -k file://$iorfile -m none ");
$sv = $SV->SpawnWaitKill ($PerlACE::wait_interval_for_process_creation);

if ($sv != 0) {
    print STDERR "ERROR in Collocation_Oneway_Test\n";
    $status = 1;
}
unlink $iorfile;

print STDERR "======== Running in Default Mode \n";
$SV->Arguments ("-o $iorfile -k file://$iorfile -m transport");
$sv = $SV->SpawnWaitKill ($PerlACE::wait_interval_for_process_creation);

if ($sv != 0) {
    print STDERR "ERROR in Collocation_Oneway_Test\n";
    $status = 1;
}
unlink $iorfile;

print STDERR "======== Running in Default Mode \n";
$SV->Arguments ("-o $iorfile -k file://$iorfile -m server");
$sv = $SV->SpawnWaitKill ($PerlACE::wait_interval_for_process_creation);

if ($sv != 0) {
    print STDERR "ERROR in Collocation_Oneway_Test\n";
    $status = 1;
}
unlink $iorfile;

print STDERR "======== Running in Default Mode \n";
$SV->Arguments ("-o $iorfile -k file://$iorfile -m target");
$sv = $SV->SpawnWaitKill ($PerlACE::wait_interval_for_process_creation);

if ($sv != 0) {
    print STDERR "ERROR in Collocation_Oneway_Test\n";
    $status = 1;
}
unlink $iorfile;

exit $status;
