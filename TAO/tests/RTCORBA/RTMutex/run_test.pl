eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../bin';
require ACEutils;
use Cwd;

$cwd = getcwd();

ACE::checkForTarget($cwd);

print STDERR "\n********** RTCORBA RTMutex Unit Test **********\n\n";
$server_args="";

$SV = Process::Create ($EXEPREFIX."server$EXE_EXT ",
                       $server_args);

$server = $SV->TimedWait (60);
if ($server == -1) {
  print STDERR "ERROR: server timedout\n";
  $SV->Kill (); $SV->TimedWait (1);
}

if ($server != 0) {
  print STDERR "ERROR: server returned $server\n";
  exit 1;
}

exit 0;
