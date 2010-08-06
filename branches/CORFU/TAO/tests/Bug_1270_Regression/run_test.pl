eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("server.ior");
unlink $iorfile;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "-o server.ior");
}
else {
    $SV = new PerlACE::Process ("server", "-o $iorfile");
}
$CL1 = new PerlACE::Process ("client", "-k file://$iorfile");
$CL2 = new PerlACE::Process ("client", "-k file://$iorfile");
$CL3 = new PerlACE::Process ("client", "-k file://$iorfile");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

local $start_time = time();
local $max_running_time = 240; # 4 minutes
local $elapsed = time() - $start_time;

if ($ARGV[0] eq '-quick')  {
    $elapsed = 0;
    $max_running_time = 1;
}

while($elapsed < $max_running_time) {
  # Start all clients in parallel
  $client1 = $CL1->Spawn ();
  $client2 = $CL2->Spawn ();
  $client3 = $CL3->Spawn ();

  # ... wait for the clients to die unless they did not startup,
  # ignore errors because they intentionally crash themselves!
  $CL3->WaitKill(60) unless $client3 < 0;
  $CL2->WaitKill(60) unless $client2 < 0;
  $CL1->WaitKill(60) unless $client1 < 0;

  $elapsed = time() - $start_time;
}

$SH = new PerlACE::Process ("shutdown", "-k file://$iorfile");
my $shutdown = $SH->Spawn();
$SH->WaitKill(10) unless $shutdown < 0;

$server = $SV->WaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
