eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../../../bin";
use PerlACE::Run_Test;

$con_count = 10;
$sup_count = 10;
$ev_count = 8;
$status = 0;

$nsior = PerlACE::LocalFile ("ns.ior");

unlink $nsior;

# Parse the arguments

for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") {
        print "usage: run_test2.pl -e event_count -c consumers -s suppliers -h help\n";
        exit;
    }
    elsif ($ARGV[$i] eq "-e") {
        $ev_count = $ARGV[$i + 1];
        $i++;
    }
    elsif ($ARGV[$i] eq "-c") {
        $con_count = $ARGV[$i + 1];
        $i++;
    }
    elsif ($ARGV[$i] eq "-s") {
        $sup_count = $ARGV[$i + 1];
        $i++;
    }
}

$NS  = new PerlACE::Process ("../../../../../Naming_Service/Naming_Service", 
                             "-o $nsior");
                             
$EC  = new PerlACE::Process ("../../../../../Event_Service/Event_Service", 
                             "-t new -ORBInitRef NameService=file://$nsior");
                             
$CE1 = new PerlACE::Process ("../../bin/RtEC_Based_CosEC", 
                             "-ORBInitRef NameService=file://$nsior");

for ($cntr1 = 0; $cntr1 < $con_count ; ++$cntr1) {
    push @CONS, (new PerlACE::Process ("Consumer", "-ORBInitRef NameService=file://$nsior -c $ev_count"));
}

for ($cntr2 = 0; $cntr2 < $con_count ; ++$cntr2) {
    push @SUPS, (new PerlACE::Process ("Supplier", "-ORBInitRef NameService=file://$nsior -c $ev_count"));
}

# first start the Naming service..
$NS->Spawn ();

sleep 10;

# now start the Rt EC..
$EC->Spawn ();

sleep 10;

# now start the CosEC1..
$CE1->Spawn ();

sleep 10;

# now start the consumers
foreach $co (@CONS) {
    print "creating consumer# $cntr1\n";
    $co->Spawn ();
    sleep 10;
}

# now start the suppliers
foreach $su (@SUPS) {
    print "creating supplier# $cntr2\n";
    $su->Spawn ();
    sleep 10;
}

print "waiting for the suppliers to finish\n";
foreach $su (@SUPS) {
    $supplier = $su->WaitKill (60);
    
    if ($supplier != 0) {
        print STDERR "ERROR: A supplier returned $supplier\n";
        $status = 1;
    }
}


print "waiting for the last consumer to finish\n";
foreach $co (@CONS) {
    $consumer = $co->WaitKill (60);
    
    if ($consumer != 0) {
        print STDERR "ERROR: A consumer returned $consumer\n";
        $status = 1;
    }
}

print "cleanup...\n";
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
print "done!.\n";

unlink $nsior;

exit $status;
