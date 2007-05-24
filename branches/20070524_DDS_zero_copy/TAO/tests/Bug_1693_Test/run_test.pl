eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-
# $Id$

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

print STDERR "\n\n==== Running Bug_1693_Test test\n";

if (PerlACE::is_vxworks_test()) {
    $T = new PerlACE::ProcessVX ("client");
}
else {
    $T = new PerlACE::Process ("client");
}

$test = $T->SpawnWaitKill ($PerlACE::wait_interval_for_process_creation);

if ($test != 0) {
    print STDERR "ERROR: Bug 1693 test returned $test\n";
    exit 1;
}

exit 0;
