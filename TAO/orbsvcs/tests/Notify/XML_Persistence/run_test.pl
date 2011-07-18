eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# ex

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use File::Compare;

$port = 12000 + PerlACE::uniqueid ();

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

$UTEST = new PerlACE::Process("main", "-orbobjrefstyle url -ORBEndpoint iiop://:$port -pass 3");
$ret = $UTEST->SpawnWaitKill(20);
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
}


exit $ret;
