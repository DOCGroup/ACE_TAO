eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../bin';
require ACEutils;
require Process;
use Cwd;

$status = 0;

ACE::checkForTarget(getcwd());

$TEST = Process::Create ($EXEPREFIX."EC_Basic".$EXE_EXT, "");

if ($TEST->TimedWait (60) == -1) {
  print STDERR "ERROR: test timedout\n";
  $status = 1;
  $TEST->Kill (); $TEST->TimedWait (1);
}

# @@ Capture any errors from the server too.
exit $status;
