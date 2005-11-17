eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../../../bin";
use PerlACE::Run_Test;

#event count
$ev_count = 20;
$status = 0;

# setup CosEC params such that..
# cos event service name = "cosec1"
# for ConsumerQOS: EventID = 21, SourceID = 6
# for SupplierQOS: EventID = 20, SourceID = 5.
$CosEC1_params = "-n cosec1 -e 21 -o 6 -p \"5 20\"";

# setup another CosEC params such that..
# cos event service name = "cosec2"
# for ConsumerQOS: EventID = 20, SourceID = 5
# for SupplierQOS: EventID = 21, SourceID = 6.
$CosEC2_params = "-n cosec2 -e 20 -o 5 -p \"6 21\"";

$nsior = PerlACE::LocalFile ("ns.ior");

unlink $nsior;

# Parse the arguments

for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") {
        print "usage: run_test.pl -e event_count -h help\n";
        exit;
    }
    elsif ($ARGV[$i] eq "-e") {
        $ev_count = $ARGV[$i + 1];
        $i++;
    }
}

$NS  = new PerlACE::Process ("../../../../../Naming_Service/Naming_Service", 
                             "-o $nsior");
                             
$EC  = new PerlACE::Process ("../../../../../Event_Service/Event_Service", 
                             "-t new -ORBInitRef NameService=file://$nsior");
                             
$CE1 = new PerlACE::Process ("../../bin/RtEC_Based_CosEC", 
                             "-ORBInitRef NameService=file://$nsior $CosEC1_params");
                             
$CE2 = new PerlACE::Process ("../../bin/RtEC_Based_CosEC", 
                             "-ORBInitRef NameService=file://$nsior $CosEC2_params");
                             
$CO1 = new PerlACE::Process ("Consumer", 
                             "-ORBInitRef NameService=file://$nsior -n cosec1 -c $ev_count");
                             
$SU1 = new PerlACE::Process ("Supplier", 
                             "-ORBInitRef NameService=file://$nsior -n cosec2 -c $ev_count");
                             
$CO2 = new PerlACE::Process ("Consumer", 
                             "-ORBInitRef NameService=file://$nsior -n cosec2 -c $ev_count");
                             
$SU2 = new PerlACE::Process ("Supplier", 
                             "-ORBInitRef NameService=file://$nsior -n cosec1 -c $ev_count");

# first start the Naming service..
$NS->Spawn ();

sleep 10;

# now start the Rt EC..
$EC->Spawn ();

sleep 10;

# now start the CosEC1..
$CE1->Spawn ();

sleep 10;

# now start the CosEC2..
$CE2->Spawn ();

sleep 10;

# start 1 consumer that uses CosEC1 to receive events
$CO1->Spawn ();

sleep 10;

# start 1 supplier  that uses CosEC2 to send events
$SU1->Spawn ();

sleep 10;

# wait for the supplier to finish
$supplier = $SU1->WaitKill (60);

if ($supplier != 0) {
    print STDERR "ERROR: supplier returned $supplier\n";
    $status = 1;
}

# wait for the consumer to finish
$consumer = $CO1->WaitKill (60);

if ($consumer != 0) {
    print STDERR "ERROR: consumer returned $consumer\n";
    $status = 1;
}

#----------

# start 1 consumer that uses CosEC1 to receive events
$CO2->Spawn ();

sleep 10;

# start 1 supplier  that uses CosEC2 to send events
$SU2->Spawn ();

sleep 10;

# wait for the supplier to finish
$supplier = $SU2->WaitKill (60);

if ($supplier != 0) {
    print STDERR "ERROR: supplier returned $supplier\n";
    $status = 1;
}

# wait for the consumer to finish

$consumer = $CO2->WaitKill (60);

if ($consumer != 0) {
    print STDERR "ERROR: consumer returned $consumer\n";
    $status = 1;
}


#----------

# cleanup..

$server = $CE2->TerminateWaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: CosEC2 returned $server\n";
    $status = 1;
}

$CE1->TerminateWaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: CosEC1 returned $server\n";
    $status = 1;
}

$EC->TerminateWaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: EC returned $server\n";
    $status = 1;
}

$NS->TerminateWaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: NS returned $server\n";
    $status = 1;
}

unlink $nsior;

exit $status;
