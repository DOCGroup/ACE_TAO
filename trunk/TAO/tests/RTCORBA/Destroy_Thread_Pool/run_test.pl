eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

$continuous = ($^O eq 'hpux');

print STDERR "\n********** RTCORBA Destroy_Thread_Pool Unit Test **********\n\n";

if (PerlACE::is_vxworks_test()) {
    $T = new PerlACE::ProcessVX ("Destroy_Thread_Pool");
}
else {
    $T = new PerlACE::Process ("Destroy_Thread_Pool");    
}

if ($continuous) {
  $T->Arguments("-ORBSvcConf continuous$PerlACE::svcconf_ext");
}

$test = $T->SpawnWaitKill (60);

if ($test != 0) {
    print STDERR "ERROR: test returned $test\n";
    exit 1;
}

exit 0;
