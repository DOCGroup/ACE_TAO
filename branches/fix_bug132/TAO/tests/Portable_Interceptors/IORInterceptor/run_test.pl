eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-
# $Id$

unshift @INC, '../../../../bin';
require Process;
require ACEutils;
use Cwd;

$cwd = getcwd();
ACE::checkForTarget($cwd);

print STDERR "\n\n==== Running IORInterceptor test\n";

$file = "$cwd$DIR_SEPARATOR" . "test.ior";

unlink $file;

$SV = Process::Create ($EXEPREFIX."server".$EXE_EXT,
                       "-o $file");
if (ACE::waitforfile_timed ($file, 15) == -1) {
  print STDERR "ERROR: cannot find file <$file>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL = Process::Create ($EXEPREFIX."client".$EXE_EXT,
                       "-k file://$file");


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
