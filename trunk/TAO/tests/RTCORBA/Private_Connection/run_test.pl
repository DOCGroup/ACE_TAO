eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../bin';
require ACEutils;
use Cwd;

$cwd = getcwd();
$iorfile1 = "$cwd$DIR_SEPARATOR" . "test1.ior";
$iorfile2 = "$cwd$DIR_SEPARATOR" . "test2.ior";

ACE::checkForTarget($cwd);

print STDERR "\n********** RTCORBA Private Connection Unit Test\n\n";

unlink $iorfile;

$SV = Process::Create ($EXEPREFIX."server$EXE_EXT ",
                       " -o $iorfile1 -p $iorfile2 ");

if (ACE::waitforfile_timed ($iorfile2, 10) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL = Process::Create ($EXEPREFIX."client$EXE_EXT ",
                       " -o file://$iorfile1 -p file://$iorfile2 "
                       ."-ORBdebuglevel 3 ");

$client = $CL->TimedWait (60);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
}

$server = $SV->TimedWait (60);
if ($server == -1) {
  print STDERR "ERROR: server timedout\n";
  $SV->Kill (); $SV->TimedWait (1);
}

unlink $iorfile1;
unlink $iorfile2;

if ($server != 0 || $client != 0) {
  exit 1;
}

exit 0;
