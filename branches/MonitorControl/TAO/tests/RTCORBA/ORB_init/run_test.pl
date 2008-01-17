eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

print STDERR "\n********** RTCORBA ORB_init Unit Test **********\n\n";

if (PerlACE::is_vxworks_test()) {
    $T = new PerlACE::ProcessVX ("ORB_init");
}
else {
    $T = new PerlACE::Process ("ORB_init");
}

$test = $T->SpawnWaitKill (60);

if ($test != 0) {
    print STDERR "ERROR: test returned $test\n";
    exit 1;
}

exit 0;
