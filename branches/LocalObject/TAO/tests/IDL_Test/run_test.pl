eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../bin";
require ACEutils;
require Process;

$M = Process::Create ($EXEPREFIX."main".$EXE_EXT);

$test = $M->TimedWait (10);

if ($test == -1) {
  print STDERR "ERROR: test timedout\n";
  $M->Kill (); $M->TimedWait (1);
}

