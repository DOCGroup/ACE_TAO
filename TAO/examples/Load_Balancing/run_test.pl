eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs a Load Balancing service demo.
# See README file for details about the demo.

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

# File used to pass load balancing service ior to its clients.
$iorbase = "lb.ior";
$iorfile = PerlACE::LocalFile("$iorbase");

unlink $iorfile;

if (PerlACE::is_vxworks_test()) {
  $LB = new PerlACE::ProcessVX ("load_balancer", "-o $iorbase");
}
else {
  $LB = new PerlACE::Process ("load_balancer", "-o $iorfile");
}
$SV = new PerlACE::Process ("server", "-i file://$iorfile");
$CL = new PerlACE::Process ("client", "-i file://$iorfile -n 10");

print STDERR "\n    Starting Load Balancing Server and Identity Server \n\n";

# Run the load balancing server.
$LB->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: File containing Load Balancing Service ior,".
        " <$iorfile>, cannot be found\n";
    $LB->Kill ();
    exit 1;
}

# Run the identity server, which registers its objects with the load
# balancing server.

$SV->Spawn ();
sleep ($PerlACE::wait_interval_for_process_creation);


# Run tests, i.e., run client with different command line options.

print STDERR "\n     Client using Round Robin Object Group (10 iterations): \n\n";
$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$CL->Arguments ("-r " . $CL->Arguments ());

print STDERR "\n     Client using Random Object Group (10 iterations): \n\n";
$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

# Clean up.
$loadbalancer= $LB->TerminateWaitKill (15);

if ($loadbalancer != 0) {
    print STDERR "ERROR: load balancer returned $loadbalancer\n";
    $status = 1;
}

$server = $SV->TerminateWaitKill (15);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}
unlink $iorfile;

exit $status;
