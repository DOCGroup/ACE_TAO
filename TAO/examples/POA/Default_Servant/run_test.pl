eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../bin";
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("server.ior");

unlink $iorfile;

$SV = new PerlACE::Process ("server", "-o $iorfile");
$CL = new PerlACE::Process ("client", "-k $iorfile");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 5) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill ();
  exit 1;
}

$client = $CL->SpawnWaitKill (60);

if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
}

$SV->Kill ();

unlink $iorfile;
unlink "test"; # created by the program

exit $client;
