eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test

$T = new PerlACE::Process ("registration");

$test = $T->SpawnWaitKill (60);

if ($test != 0) {
    print STDERR "ERROR: test returned $test\n";
    exit 1;
}

exit 0;
