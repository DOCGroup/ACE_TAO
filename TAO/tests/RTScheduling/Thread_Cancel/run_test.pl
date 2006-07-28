eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

if (PerlACE::is_vxworks_test()) {
    $CL = new PerlACE::ProcessVX ("Thread_Cancel");
}
else {
    $CL = new PerlACE::Process ("Thread_Cancel");
}

print STDERR "Starting Current Test\n";

$test = $CL->SpawnWaitKill (200);

if ($test != 0) {
    print STDERR "ERROR: test returned $test\n";
    $status = 1;
}

exit $status;
