eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$ 
# -*- perl -*-

unshift @INC, '../../../../bin';
require ACEutils;
require Process;

$status = 0;
$iorfile = "time.ior";

$SV = Process::Create ($EXEPREFIX."server$EXE_EXT", "-o $iorfile -ORBendpoint diop://:12345");

if (ACE::waitforfile_timed ($iorfile, 5) == -1) {
  print STDERR "ERROR: timedout waiting for file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$client = Process::Create ($EXEPREFIX."client$EXE_EXT", "-k file://$iorfile -t 10 -i 1000");
if ($client->TimedWait (60) == -1) {
  print STDERR "ERROR: client timedout\n";
  $status = 1;
  $client->Kill (); $client->TimeWait (1);
}

if ($SV->TimedWait (1) == -1) {
  print STDERR "ERROR: cannot terminate the server\n";
  $SV->Kill (); $SV->TimedWait (1);
  $status = 1;
}

unlink $iorfile;

exit $status;
