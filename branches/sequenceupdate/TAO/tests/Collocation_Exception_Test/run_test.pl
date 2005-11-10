eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("test.ior");
unlink $iorfile;
$status = 0;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("Collocated_Test");
}
else {
    $SV = new PerlACE::Process ("Collocated_Test");
}

print STDERR "======== Running in Default Mode \n";
$SV->Arguments ("-o $iorfile -k file://$iorfile");
$sv = $SV->SpawnWaitKill (60);

if ($sv != 0) {
    print STDERR "ERROR in Collocation_Exception_Test\n";
    $status = 1;
}
unlink $iorfile;

exit $status;
