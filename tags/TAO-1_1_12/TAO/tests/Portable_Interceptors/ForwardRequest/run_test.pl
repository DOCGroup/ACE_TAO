eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-
#
# $Id$


unshift @INC, '../../../../bin';
require Process;
require ACEutils;
use Cwd;

$cwd = getcwd();
ACE::checkForTarget($cwd);

print STDERR "\n\n==== Running PortableInterceptor::ForwardRequest test\n";

$file1 = "$cwd$DIR_SEPARATOR" . "test1.ior";
$file2 = "$cwd$DIR_SEPARATOR" . "test2.ior";

unlink $file1;
unlink $file2;

$SV = Process::Create ($EXEPREFIX."server".$EXE_EXT,
                       "-o $file1 -o $file2");
if (ACE::waitforfile_timed ($file1, 15) == -1
    || ACE::waitforfile_timed ($file2, 15) == -1) {
  print STDERR "ERROR: cannot find file <$file1> or <$file2>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL = Process::Create ($EXEPREFIX."client".$EXE_EXT,
                       "-k file://$file1 -k file://$file2");


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

unlink $file1;
unlink $file2;

exit 0;
