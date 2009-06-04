eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$file = PerlACE::LocalFile ("test.ior");

unlink $file;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "");
}
else {
    $SV = new PerlACE::Process ("server", "");
}

print STDERR "\n\n==== Running bug 3646d regression test\n";

$SV->Spawn ();

$collocated = $SV->WaitKill (200);

if ($collocated != 0) {
    print STDERR "ERROR: Bug_3646d_Regression returned $collocated\n";
    $status = 1;
}

unlink $file;

exit $status;
