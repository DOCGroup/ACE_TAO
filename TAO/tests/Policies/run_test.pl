eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../bin';
require ACEutils;
use Cwd;

$cwd = getcwd();
$threads='10';
$status = 0;

$T = Process::Create ($EXEPREFIX."Manipulation$EXE_EXT",
		      " -n $threads");

$t = $T->TimedWait (60);
if ($t == -1) {
  print STDERR "ERROR: test timedout\n";
  $T->Kill (); $T->TimedWait (1);
  $status = 1;
}

exit $status;
