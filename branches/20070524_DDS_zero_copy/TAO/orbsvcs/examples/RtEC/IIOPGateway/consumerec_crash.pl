eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

$ns_ior   = PerlACE::LocalFile ("ns.ior");
$conffile = PerlACE::LocalFile ("ec" . "$PerlACE::svcconf_ext");
$gatewayconffile = PerlACE::LocalFile ("gateway" . "$PerlACE::svcconf_ext");

unlink $ns_ior;

$NS = new PerlACE::Process ("../../../Naming_Service/Naming_Service",
                           "-o $ns_ior");

$T1 = new PerlACE::Process ("EC",
                            "-ORBInitRef NameService=file://$ns_ior "
                            . "-ORBsvcconf $conffile "
#                            . "-ORBDebug -ORBDebugLevel 10 -ORBLogFile supplierec.log "
                            . "-e channel1 ");

$T2 = new PerlACE::Process ("EC",
                            "-ORBInitRef NameService=file://$ns_ior -ORBEndpoint iiop://localhost:6000 "
                            . "-ORBsvcconf $conffile "
#                            . "-ORBDebug -ORBDebugLevel 10 -ORBLogFile consumerec.log "
                            . "-e channel2 ");

$G = new PerlACE::Process ("Gateway",
                           "-ORBInitRef NameService=file://$ns_ior "
                           . "-ORBSvcconf $gatewayconffile "
                           . "-c channel2 "
#                           . "-ORBDebug -ORBDebugLevel 10 -ORBLogFile gateway.log "
                           . "-s channel1 ");

$C = new PerlACE::Process ("Consumer",
                           "-ORBInitRef NameService=file://$ns_ior "
#                              . "-ORBDebug -ORBDebugLevel 10 -ORBLogFile consumer.log "
                           . "-e channel2 ");

$S = new PerlACE::Process ("Supplier",
                           "-ORBInitRef NameService=file://$ns_ior "
#                           . "-ORBDebug -ORBDebugLevel 10 -ORBLogFile supplier.log "
                           . "-e channel1 ");

print STDOUT "Starting name server\n";
$NS->Spawn ();

if (PerlACE::waitforfile_timed ($ns_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$ns_ior>\n";
    $NS->Kill (); 
    exit 1;
}

print STDOUT "Starting event channel 1\n";
$T1->Spawn ();

sleep 2;

print STDOUT "Starting event channel 2\n";
$T2->Spawn ();

sleep 2;

print STDOUT "Starting gateway\n";
$G->Spawn ();

sleep 2;

print STDOUT "Starting consumer\n";
$C->Spawn ();

sleep 1;

print STDOUT "Starting supplier\n";
#$supplier = $S->SpawnWaitKill (12000);
$S->Spawn();

sleep 1;

if ($supplier != 0) {
    print STDERR "ERROR: supplier returned $supplier\n";
    $status = 1;
}

#$consumer = $C->WaitKill (10);
#
#if ($consumer != 0) {
#    print STDERR "ERROR: consumer returned $consumer\n";
#    $status = 1;
#}

print STDOUT "Terminating event channel 2  and consumer in 10 seconds...\n";
#$service = $T2->TerminateWaitKill (5);
$service = $T2->WaitKill (10);
$C->Kill();

if ($service != 0) {
    print STDERR "ERROR: service returned $service\n";
    $status = 1;
}

sleep 10;

print STDOUT "Starting event channel 2 again...\n";
$T2->Spawn ();

sleep 2;

print STDOUT "Starting consumer again...\n";
$C->Spawn ();

#$supplier = $C->WaitKill (15);
#
#if ($supplier != 0) {
#    print STDERR "ERROR: supplier returned $supplier\n";
#    $status = 1;
#}

print STDOUT "1500 seconds before termination...\n";
sleep 1500;

print STDOUT "Terminating supplier...\n";
$S->TerminateWaitKill (5);

print STDOUT "Terminating consumer...\n";
$C->TerminateWaitKill (5);

print STDOUT "Terminating gateway...\n";
$G->TerminateWaitKill (5);

print STDOUT "Terminating event channels...\n";
$T1->TerminateWaitKill (5);
$T2->TerminateWaitKill (5);

$nserver = $NS->TerminateWaitKill (5);

if ($nserver != 0) {
    print STDERR "ERROR: name server returned $nserver\n";
    $status = 1;
}

unlink $ns_ior;

exit $status;
