eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../bin";
require ACEutils;
require Process;

$iorfile = "grid.ior";
$mem     = "foo";

$SV = Process::Create ($EXEPREFIX."server$Process::EXE_EXT", "-o $iorfile -m $mem");

if (ACE::waitforfile_timed ($iorfile, 10) == -1) {
  print STDERR "ERROR: timedout waiting for file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$client  = Process::Create ($EXEPREFIX."client$Process::EXE_EXT -k file://$iorfile ");
if ($client->TimedWait (60) == -1) {
  print STDERR "ERROR: client timedout\n";
  $status = 1;
  $client->Kill (); $client->TimeWait (1);
}

$SV->Kill (); $SV->TimedWait (1);

unlink $iorfile;

$server = Process::Create ($EXEPREFIX."server$Process::EXE_EXT", "-o $iorfile -m $mem");

if (ACE::waitforfile_timed ($iorfile, 10) == -1) {
  print STDERR "ERROR: timedout waiting for file <$iorfile>\n";
  $server->Kill (); $server->TimedWait (1);
  exit 1;
}

$per_client  = Process::Create ($EXEPREFIX."persistent_client$Process::EXE_EXT -k file://$iorfile -w 10 -h 10");

if ($per_client->TimedWait (60) == -1) {
  print STDERR "ERROR: client timedout\n";
  $status = 1;
  $perclient->Kill (); $per_client->TimeWait (1);
}

$server->Kill (); $server->TimedWait (1);

unlink $iorfile;
unlink $mem;
exit $status;
