eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../bin";
use PerlACE::Run_Test;
use Cwd;

$iorfile = PerlACE::LocalFile ("ior");

unlink $iorfile;

$SV = new PerlACE::Process ("server", "-o $iorfile");
$CL = new PerlACE::Process ("client", "-k file://$iorfile -x");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 5) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$client = $CL->SpawnWaitKill (200);

if ($client == -1) {
  $time = localtime;
  print STDERR "ERROR: client timedout at $time\n";
}

$server = $SV->WaitKill (100);
if ($server == -1) {
  $time = localtime;
  print STDERR "ERROR: server timedout at $time\n";
}

unlink $iorfile;

if ($server != 0 || $client != 0) {
  exit 1;
}

exit 0;
