eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

my($T)      = new PerlACE::Process ("test");
my($test)   = $T->SpawnWaitKill (20);
my($status) = 0;

if ($test != 0) {
  print STDERR "ERROR: Test failed: $test\n";
  ++$status;
}

exit $status;
