eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../bin';
require ACEutils;
use Cwd;

$cwd = getcwd();
for($i = 0; $i <= $#ARGV; $i++) {
  if ($ARGV[$i] eq '-chorus') {
    $i++;
    if (defined $ARGV[$i]) {
      $EXEPREFIX = "rsh $ARGV[$i] arun $cwd$DIR_SEPARATOR";
    }
    else {
      print STDERR "The -chorus option requires the hostname of the target\n";
      exit(1);
    }
  }
}

$orb_init = Process::Create ($EXEPREFIX."ORB_init$EXE_EXT ");


$pid = $orb_init->TimedWait (15);
if ($pid == -1) {
  print STDERR "ERROR: ORB_init timed out\n";
  $orb_init->Kill (); $orb_init->TimedWait (1);

  exit 1;
}

exit 0;
