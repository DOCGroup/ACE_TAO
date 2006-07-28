eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$threads = '10';

if (PerlACE::is_vxworks_test()) {
    $T = new PerlACE::ProcessVX ("Manipulation", "-n $threads");
}
else {
    $T = new PerlACE::Process ("Manipulation", "-n $threads");
}

$test = $T->SpawnWaitKill (60);

if ($test != 0) {
    print STDERR "ERROR: test returned $test\n";
    exit 1;
}

exit 0;
