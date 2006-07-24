eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;

$status = 0;

if (PerlACE::is_vxworks_test()) {
    $T = new PerlACE::ProcessVX ("Queued_Message_Test");
}
else {
    $T = new PerlACE::Process ("Queued_Message_Test");
}

$test = $T->SpawnWaitKill (60);

if ($test != 0) {
    print STDERR "ERROR: test returned $test\n";
    $status = 1;
}

exit $status;
