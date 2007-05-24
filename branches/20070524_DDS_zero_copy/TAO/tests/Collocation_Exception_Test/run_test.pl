eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$iorbase = "test.ior";
$status = 0;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("Collocated_Test");
    $iorfile = $iorbase;
}
else {
    $SV = new PerlACE::Process ("Collocated_Test");
    $iorfile = PerlACE::LocalFile ("test.ior");
}
unlink $iorfile;
$SV->Arguments ("-o $iorfile -k file://$iorfile");

print STDERR "======== Running in Default Mode \n";
$sv = $SV->SpawnWaitKill ($PerlACE::wait_interval_for_process_creation);

if ($sv != 0) {
    print STDERR "ERROR in Collocation_Exception_Test\n";
    $status = 1;
}
unlink $iorfile;

exit $status;
