eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../bin';
require ACEutils;

$iorfile = "ior";

unlink $iorfile;

$SV = Process::Create ($EXEPREFIX."server$EXE_EXT");

if (ACE::waitforfile_timed ($iorfile, 5) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL = Process::Create ($EXEPREFIX."client$EXE_EXT ", "-x");

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
