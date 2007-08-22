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
$LS = new PerlACE::Process ("../../../Logging_Service/Notify_Logging_Service/Notify_Logging_Service", "-ORBInitRef NameService=file://$nsior");
$consumer = new PerlACE::Process ("Notify_Consumer", "-ORBInitRef NameService=file://$nsior");
$supplier = new PerlACE::Process ("Notify_Supplier", "-ORBInitRef NameService=file://$nsior");


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
# reference in the naming service.
sleep (5);

print STDERR "Starting Consumer\n";

$consumer->Spawn ();

sleep (1);

print STDERR "Starting Supplier\n";

$supplier->Spawn ();


$supplier->WaitKill (10);

$consumer->Kill ();

$NS->Kill ();

$LS->Kill ();

#if ($client != 0) {
#	print STDERR "ERROR: client returned $client\n";
#	$status = 1;
#}

unlink $nsior;

exit $status;
