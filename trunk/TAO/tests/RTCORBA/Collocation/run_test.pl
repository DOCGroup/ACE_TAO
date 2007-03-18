eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$continuous = ($^O eq 'hpux');

print STDERR "\n********** RTCORBA Collocation Unit Test **********\n\n";

if (PerlACE::is_vxworks_test()) {
    $T = new PerlACE::ProcessVX ("Collocation");
}
else {
    $T = new PerlACE::Process ("Collocation");    
}

if ($continuous) {
  $T->Arguments("-ORBSvcConf continuous$PerlACE::svcconf_ext");
}

$test = $T->SpawnWaitKill(60);
if ($test == 2) {
  # Mark as no longer running to avoid errors on exit.
  $T->{RUNNING} = 0;
} else {
  if ($test != 0) {
    print STDERR "ERROR: test returned $test\n";
    exit 1;
  }
}

exit 0;
