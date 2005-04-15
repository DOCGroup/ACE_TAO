eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

if (PerlACE::is_vxworks_test()) {
    $T = new PerlACE::ProcessVX ("Non_Servant_Upcalls");
}
else {
    $T = new PerlACE::Process ("Non_Servant_Upcalls");
}

$test = $T->SpawnWaitKill (60);

if ($test != 0) {
    print STDERR "ERROR: test returned $test\n";
    exit 1;
}

exit 0;
