eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs a Load Balancing service demo.
# See README file for details about the demo.

use lib "../../../bin";
use PerlACE::Run_Test;

# Amount of delay (in seconds) between starting a server and a client
# to allow proper server initialization.
$sleeptime = 4;

# File used to pass load balancing service ior to its clients.
$iorfile = PerlACE::LocalFile("lb.ior");

unlink $iorfile;

$LB = new PerlACE::Process ("load_balancer", "-o $iorfile");
$SV = new PerlACE::Process ("server", "-i file://$iorfile");
$CL = new PerlACE::Process ("client", "-i file://$iorfile -n 10");

print STDERR "\n    Starting Load Balancing Server and Identity Server \n\n";

# Run the load balancing server.
$LB->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $sleeptime) == -1) {
    print STDERR "ERROR: File containing Load Balancing Service ior,".
        " <$iorfile>, cannot be found\n";
    $LB->Kill ();
    exit 1;
}

# Run the identity server, which registers its objects with the load
# balancing server.

$SV->Spawn ();
sleep ($sleeptime);


# Run tests, i.e., run client with different command line options.

print STDERR "\n     Client using Round Robin Object Group (10 iterations): \n\n";
$client = $CL->SpawnWaitKill (60);

if ($client == -1) {
    print STDERR "ERROR: client timedout\n";
    $CL->Kill (); 
}

$CL->Arguments ("-r " . $CL->Arguments ());

print STDERR "\n     Client using Random Object Group (10 iterations): \n\n";
$client = $CL->SpawnWaitKill (60);

if ($client == -1) {
    print STDERR "ERROR: client timedout\n";
    $CL->Kill (); 
}


# Clean up.
$LB->Kill ();
$SV->Kill ();

# @@ Capture any exit status from the processes.
exit 0;
