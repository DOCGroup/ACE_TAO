eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

$status = 0;

$CL = new PerlACE::Process ("test", "");

print STDERR "Starting Current Test\n";

$test = $CL->SpawnWaitKill (10);

if ($test != 0) {
    print STDERR "ERROR: test returned $test\n";
    $status = 1;
}

exit $status;
