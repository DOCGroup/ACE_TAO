eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../bin";
require ACEutils;
require Process;

$status = 0;
$iorfile = "grid.ior";

$SV = Process::Create ($EXEPREFIX."server$EXE_EXT", "-o $iorfile ");

if (ACE::waitforfile_timed ($iorfile, 5) == -1) {
  print STDERR "ERROR: timedout waiting for file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$client = Process::Create($EXEPREFIX."client$EXE_EXT", "-f $iorfile -x -w 12 -h 12 -p 2 -q 2 -v 2345");

if ($client->TimedWait (60) == -1) {
  print STDERR "ERROR: client timedout\n";
  $status = 1;
  $client->Kill (); $client->TimedWait (1);
}

if ($SV->TimedWait (5) == -1) {
  print STDERR "ERROR: cannot terminate the server\n";
  $SV->Kill (); $SV->TimedWait (1);
  $status = 1;
}

unlink $iorfile;

exit $status;
