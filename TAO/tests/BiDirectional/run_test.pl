eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../bin';
require ACEutils;
use Cwd;

$cwd = getcwd();
$iorfile = "$cwd$DIR_SEPARATOR" . "test.ior";
$status = 0;

ACE::checkForTarget($cwd);

unlink $iorfile;
$SV = Process::Create ($EXEPREFIX."server$EXE_EXT ",
                       " -o $iorfile -i 100");

if (ACE::waitforfile_timed ($iorfile, 15) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL = Process::Create ($EXEPREFIX."client$EXE_EXT ",
                       " -k file://$iorfile");

$client = $CL->TimedWait (20);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
  $status = 1;
}

unlink $iorfile;

exit $status;
