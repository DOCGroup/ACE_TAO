eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../bin';
require Process;
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

# Run two copies of the same test...
$T1 = Process::Create ($EXEPREFIX."EC_Mcast".$EXE_EXT,
		       " -c $cwd$DIR_SEPARATOR" .
		       "sample.cfg -n 100 -t 50000 -f Set02");
$T2 = Process::Create ($EXEPREFIX."EC_Mcast".$EXE_EXT,
		       " -c $cwd$DIR_SEPARATOR" .
		       "sample.cfg -n 100 -t 50000 -f Set02");

if ($T1->TimedWait (60) == -1) {
  print STDERR "ERROR: test1 timedout\n";
  $T1->Kill (); $T1->TimedWait (1);
  $T2->Kill (); $T2->TimedWait (1);
  exit 1;
}

if ($T2->TimedWait (60) == -1) {
  print STDERR "ERROR: test2 timedout\n";
  $T2->Kill (); $T2->TimedWait (1);
  exit 1;
}

exit 0;
