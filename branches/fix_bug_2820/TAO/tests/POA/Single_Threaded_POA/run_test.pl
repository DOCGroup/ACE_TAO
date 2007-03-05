eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

if (PerlACE::is_vxworks_test()) {
    $T = new PerlACE::ProcessVX ("Single_Threaded_POA");
}
else {
    $T = new PerlACE::Process ("Single_Threaded_POA");
}

$client = $T->SpawnWaitKill (60);

if ($client != 0) {
  print STDERR "ERROR: test returned $client\n";
  exit 1;
}

exit 0;

