eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs some Naming Service tests.
# It runs all the tests that will run with min CORBA.
# It starts all the servers and clients as necessary.

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

# Amount of delay (in seconds) between starting a server and a client
# to allow proper server initialization.
$sleeptime = 10;

# Variables for command-line arguments to client and server
# executables.
$ns_orb_port = 12002 + PerlACE::uniqueid ();

$ior_file = "test.ior";


$status = 0;

my $args = "-ORBEndpoint iiop://127.0.0.1:$ns_orb_port -o $ior_file";
my $prog = "../../Naming_Service/Naming_Service";
$NS = new PerlACE::Process ($prog, $args);

$NS->Spawn ();

if (PerlACE::waitforfile_timed ($ior_file,
                                $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$ior_file>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
} 

my $new_args = "-ORBInitRef NameService=corbaloc:iiop:127.0.0.1:$ns_orb_port/NameService";
my $new_prog = "TimeServer";

print " ARGS are == $new_args \n";

$CL = new PerlACE::Process ($new_prog, $new_args);

$client = $CL->SpawnWaitKill (300);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}


$NS->Kill ();
unlink $ior_file;

exit $status;
