eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

if (PerlACE::is_vxworks_test()) {
    $CL = new PerlACE::ProcessVX ("client");
}
else {
    $CL = new PerlACE::Process ("client");
}
print STDERR "\n==== Running Recursive ORBInitializer test ====\n";

$client = $CL->SpawnWaitKill (5);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

exit $status;
