eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("server.ior");
unlink $iorfile;

$SV = new PerlACE::Process ("server", "-o $iorfile -ORBdotteddecimaladdresses 1");
$CL1 = new PerlACE::Process ("client", "-k file://$iorfile");
$CL2 = new PerlACE::Process ("client", "-k file://$iorfile");
$CL3 = new PerlACE::Process ("client", "-k file://$iorfile");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

local $start_time = time();
local $max_running_time = 300; # 5 minutes
local $elapsed = time() - $start_time;

while($elapsed < $max_running_time) {
  # Start all clients in parallel
  $client1 = $CL1->Spawn ();
  $client2 = $CL2->Spawn ();
  $client3 = $CL3->Spawn ();

  # ... wait for the clients to die unless they did not startup,
  # ignore errors because they intentionally crash themselves!
  $CL3->WaitKill(30) unless $client3 < 0;
  $CL2->WaitKill(30) unless $client2 < 0;
  $CL1->WaitKill(30) unless $client1 < 0;

  $elapsed = time() - $start_time;
}

$server = $SV->TerminateWaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;


exit $status;
