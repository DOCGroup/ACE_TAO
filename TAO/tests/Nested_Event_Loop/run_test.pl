eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../bin';
require ACEutils;
use Cwd;

$cwd = getcwd();
$iorfile = "$cwd$DIR_SEPARATOR" . "ior";

ACE::checkForTarget($cwd);

unlink $iorfile;

$SV = Process::Create ($EXEPREFIX."server$EXE_EXT", "-o $iorfile");

if (ACE::waitforfile_timed ($iorfile, 5) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL = Process::Create ($EXEPREFIX."client$EXE_EXT ",
                       "-k file://$iorfile -x");

$client = $CL->TimedWait (200);
if ($client == -1) {
  $time = localtime;
  print STDERR "ERROR: client timedout at $time\n";
  $CL->Kill (); $CL->TimedWait (1);
}

$server = $SV->TimedWait (100);
if ($server == -1) {
  $time = localtime;
  print STDERR "ERROR: server timedout at $time\n";
  $SV->Kill (); $SV->TimedWait (1);
}

unlink $iorfile;

if ($server != 0 || $client != 0) {
  exit 1;
}

exit 0;
