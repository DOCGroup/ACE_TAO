eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

$status = 0;

$ec_conf = PerlACE::LocalFile ("ec$PerlACE::svcconf_ext");
$ec_mt_conf = PerlACE::LocalFile ("ec$PerlACE::svcconf_ext");

print STDERR "================ Collocated tests, single threaded\n";

$T = new PerlACE::Process ("ECT_Throughput",
                           "-ORBSvcConf $ec_conf"
                           . " -m new -u 10000 -n 1 -t 0 -c 4");


$test = $T->SpawnWaitKill (120);

if ($test != 0) {
    print STDERR "ERROR: test returned $test\n";
    $status = 1;
}

print STDERR "================ Collocated tests, multi threaded\n";

$T = new PerlACE::Process ("ECT_Throughput",
                           "-ORBSvcConf $ec_mt_conf" 
                           . " -m new -u 10000 -n 1 -t 0 -c 4");

$test = $T->SpawnWaitKill (60);

if ($test != 0) {
    print STDERR "ERROR: test returned $test\n";
    $status = 1;
}

print STDERR "================ Remote test\n";

$ns_ior = PerlACE::LocalFile ("NameService.ior");

unlink $ns_ior;

$NS = new PerlACE::Process ("../../Naming_Service/Naming_Service",
                           "-o $ns_ior");
                           
$ES = new PerlACE::Process ("../../Event_Service/Event_Service",
                            "-ORBInitRef NameService=file://$ns_ior "
                            . " -ORBSvcConf $ec_conf "
                            . " -t NEW");
                            
$C = new PerlACE::Process ("ECT_Consumer",
                           "-ORBInitRef NameService=file://$ns_ior "
                           . " -c 4 -s 1");

$S = new PerlACE::Process ("ECT_Supplier",
                           "-ORBInitRef NameService=file://$ns_ior "
                           . " -s 1 -u 5000 -n 1 -t 0");

$NS->Spawn ();

if (PerlACE::waitforfile_timed ($ns_ior, 10) == -1) {
    print STDERR "ERROR: cannot find file <$ns_ior>\n";
    $NS->Kill (); 
    exit 1;
}

$ES->Spawn ();

sleep 10;

$C->Spawn ();
$S->Spawn ();

$supplier = $S->WaitKill (300);

if ($supplier != 0) {
    print STDERR "ERROR: supplier returned $supplier\n";
    $status = 1;
}

$consumer = $C->WaitKill (60);

if ($consumer != 0) {
    print STDERR "ERROR: consumer returned $consumer\n";
    $status = 1;
}

$eserver = $ES->WaitKill (5);

if ($eserver != 0) {
    print STDERR "ERROR: event server returned $eserver\n";
    $status = 1;
}

$nserver = $NS->TerminateWaitKill (5);

if ($nserver != 0) {
    print STDERR "ERROR: name server returned $nserver\n";
    $status = 1;
}

unlink $ns_ior;

exit $status;
