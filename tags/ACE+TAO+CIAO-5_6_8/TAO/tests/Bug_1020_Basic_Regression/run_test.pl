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
$CL = new PerlACE::Process ("client", "-k file://$iorfile");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

local $start_time = time();
local $max_running_time = 300; # 5 minutes
local $elapsed = time() - $start_time;

if ($ARGV[0] eq '-quick')  {
    $elapsed = 0;
    $max_running_time = 1;
}

    print STDERR "elaped is $elapsed\n";
while($elapsed < $max_running_time) {
    print STDERR "**************************** \n";
  # Start all clients in parallel
  $client = $CL->Spawn ();

  # ... wait for the clients to die unless they did not startup,
  # ignore errors because they intentionally crash themselves!
  $CL->WaitKill(30) unless $client < 0;

  $elapsed = time() - $start_time;
}

$server = $SV->TerminateWaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
