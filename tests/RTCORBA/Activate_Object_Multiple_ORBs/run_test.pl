eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

if (PerlACE::is_vxworks_test()) {
$T      = new PerlACE::ProcessVX ("server");
}
else {
$T      = new PerlACE::Process ("server");
}
my($test)   = $T->SpawnWaitKill ($PerlACE::wait_interval_for_process_creation);
my($status) = 0;

if ($test != 0) {
  print STDERR "ERROR: Test failed: $test\n";
  ++$status;
}

exit $status;
