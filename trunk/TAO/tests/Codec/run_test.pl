eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-
# $Id$

unshift @INC, '../../../bin';
require Process;
require ACEutils;
use Cwd;

$cwd = getcwd();
ACE::checkForTarget($cwd);

print STDERR "\n\n==== Running Codec test\n";

$codec = Process::Create ($EXEPREFIX."client".$EXE_EXT);

$pid = $codec->TimedWait (15);
if ($pid == -1) {
  print STDERR "ERROR: Codec timed out\n";
  $codec->Kill (); $codec->TimedWait (1);

  exit 1;
}

exit 0;
