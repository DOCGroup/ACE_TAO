eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../bin';
require Process;
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

$T = Process::Create ($EXEPREFIX."Destruction".$EXE_EXT);

$client = $T->TimedWait (60);
if ($client == -1) {
  print STDERR "ERROR: test timedout\n";
  $T->Kill (); $T->TimedWait (1);
}

if ($client == -1) {
  exit 1;
}

exit 0;
