eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

$status = 0;

$iorfile = PerlACE::LocalFile ("test.ior");

unlink $iorfile;

print STDERR "\n            RTCORBA CLIENT_PROPAGATED Priority Unit Test\n\n";

$SV = new PerlACE::Process ("server", "-o $iorfile");
$CL = new PerlACE::Process ("client", "-k file://$iorfile");

$SV->Spawn ();
$server = $SV->TimedWait(10);
if ($server == 2) {
    print STDOUT "Could not change priority levels.  Check user permissions.  Exiting...\n";
    # Mark as no longer running to avoid errors on exit.
    $SV->{RUNNING} = 0;
} else {

  if (PerlACE::waitforfile_timed ($iorfile, 10) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); 
    exit 1;
  }
  
  $client = $CL->SpawnWaitKill (60);
  
  if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
  }

  $server = $SV->WaitKill (60);

  if ($server !=0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
  }

  unlink $iorfile;
}

exit $status;
