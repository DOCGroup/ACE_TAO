eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;  
 
# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

$nsior = PerlACE::LocalFile ("ns.ior");

unlink $nsior;

$NS = new PerlACE::Process ("../../../Naming_Service/Naming_Service", "-o $nsior");
$LS = new PerlACE::Process ("../../../Logging_Service/Basic_Logging_Service/Basic_Logging_Service", "-ORBInitRef NameService=file://$nsior");
$CLIENT = new PerlACE::Process ("client", "-ORBInitRef NameService=file://$nsior");


print STDERR "Starting Naming Service\n";

$NS->Spawn ();

if (PerlACE::waitforfile_timed ($nsior, $PerlACE::wait_interval_for_process_creation) == -1) {
	print STDERR "ERROR: cannot find naming service IOR file\n";
	$NS->Kill ();
	exit 1;
}

print STDERR "Starting Logging Service\n";

$LS->Spawn ();

# Give time for logging service to initialize and install its object 
# reference in the nameing service.
sleep (5);

print STDERR "Starting client\n";

$CLIENT->Spawn ();

$client = $CLIENT->WaitKill (10);

$NS->Kill ();

$LS->Kill ();

if ($client != 0) {
	print STDERR "ERROR: client returned $client\n";
	$status = 1;
}

unlink $nsior;

exit $status;
