eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../bin';
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

print STDERR "\n\n==== Running interceptor test\n";

$file = "$cwd$DIR_SEPARATOR" . "test.ior";

unlink $file;

$SV = Process::Create ($EXEPREFIX."server".$EXE_EXT,
                       "-ORBobjrefstyle url -o $file");
if (ACE::waitforfile_timed ($file, 15) == -1) {
  print STDERR "ERROR: cannot find file <$file>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL = Process::Create ($EXEPREFIX."client".$EXE_EXT,
                       "-ORBobjrefstyle url -f file://$file");


$client = $CL->TimedWait (60);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
}

$server = $SV->TimedWait (5);
if ($server == -1) {
  print STDERR "ERROR: server timedout\n";
  $SV->Kill (); $SV->TimedWait (1);
}

if ($client == -1 || $server == -1) {
  exit 1;
}

unlink $file;

exit 0;
