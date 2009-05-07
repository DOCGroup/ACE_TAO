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
    $SV = new PerlACE::ProcessVX ("server", "-o server.ior -ORBdotteddecimaladdresses 1");
}
else {
    $SV = new PerlACE::Process ("server", "-o $iorfile -ORBdotteddecimaladdresses 1");
}
$CL1 = new PerlACE::Process ("client", "-k file://$iorfile");
$CL2 = new PerlACE::Process ("client", "-k file://$iorfile");
$CL3 = new PerlACE::Process ("client", "-k file://$iorfile");

local $start_time = time();
local $max_running_time = 600; # 5 minutes
local $elapsed = time() - $start_time;

if ($ARGV[0] eq '-quick')  {
    $elapsed = 0;
    $max_running_time = 1;
}

while($elapsed < $max_running_time) {

print STDERR " Going .. \n";
$sv1 = $SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
# Start all clients in parallel
$client1 = $CL1->Spawn ();
$client2 = $CL2->Spawn ();
$client3 = $CL3->Spawn ();


sleep (90);
$SV->WaitKill (3) unless $sv1 < 0;
$CL1->WaitKill (3);
$CL2->WaitKill (3);
$CL3->WaitKill (3);

if ($client3 != 0){
    $status = -1;
    exit $status;
}

if ($client2 != 0) {
    $status = -1;
    exit $status;
}

if ($client1 != 0){
    $status = -1;
    exit $status;
}
  $elapsed = time() - $start_time;
unlink $iorfile;
}



exit $status;
