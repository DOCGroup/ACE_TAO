eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../bin';
require ACEutils;

$orb_init = Process::Create ($EXEPREFIX."ORB_init$EXE_EXT ");


$pid = $orb_init->TimedWait (15);
if ($pid == -1) {
  print STDERR "ERROR: ORB_init timed out\n";
  $orb_init->Kill (); $orb_init->TimedWait (1);

  exit 1;
}

exit 0;
