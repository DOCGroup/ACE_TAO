eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# ex

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use File::Compare;

$ACE_ROOT = $ENV{ACE_ROOT};
$TAO_ROOT = "$ACE_ROOT/TAO";

sub cleanup() {
  unlink "loadtest.xml";
  unlink "abc.xml";
  for ($i = 0; $i < 10; ++$i) {
    unlink "abc.00" . $i;
  }
  for ($i = 10; $i < 20; ++$i) {
    unlink "abc.0" . $i;
  }
}

cleanup();

$ret = 0;

$UTEST = new PerlACE::Process("main");
$ret = $UTEST->SpawnWaitKill(10);
if ($ret != 0) {
  print "ERROR : main returned $ret\n";
  exit $ret;
}

# NOTE: In TAO 1.3a main ran the notification service twice
# producing both abc.xml and loadtest.xml.  Unfortunately
# in TAO 1.4 the notification service will does not run
# successfully the second time in the same EXE.
# -- todo figure out why and fix it.
# Thus main was modified to run in two passes.  If loadtest.xml
# exists, it runs pass 2.
$ret = $UTEST->SpawnWaitKill(10);
if ($ret != 0) {
  print "ERROR : main returned $ret\n";
  exit $ret;
}

$different = compare("loadtest.xml", "abc.xml");
if ($different) {
  print "ERROR : loadtest.xml != abc.xml\n";
  exit 1;
} else {
  print "Success : loadtest.xml == abc.xml\n";
  cleanup();
}


exit $ret;
