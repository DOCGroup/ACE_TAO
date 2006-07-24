eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

if (PerlACE::is_vxworks_test()) {
    $CL = new PerlACE::ProcessVX ("driver");
}
else {
    $CL = new PerlACE::Process ("driver");
}
print STDERR "\n==== Running Slot test ====\n";

$client = $CL->SpawnWaitKill (5);

if ($client != 0) {
    print STDERR "ERROR: driver returned $client\n";
    $status = 1;
}

exit $status;
