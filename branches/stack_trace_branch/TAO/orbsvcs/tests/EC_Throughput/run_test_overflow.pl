eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

$status = 0;

$ec_mt_conf = PerlACE::LocalFile ("ec_mt_overflow$PerlACE::svcconf_ext");


print STDERR "================ Overflow test\n";

$ns_ior = PerlACE::LocalFile ("NameService.ior");

unlink $ns_ior;

$NS = new PerlACE::Process ("../../Naming_Service/Naming_Service",
                           "-o $ns_ior");

$ES = new PerlACE::Process ("../../Event_Service/Event_Service",
                            "-ORBInitRef NameService=file://$ns_ior "
                            . " -ORBSvcConf $ec_mt_conf "
                            . " -s local");

$C = new PerlACE::Process ("ECT_Consumer",
                           "-ORBInitRef NameService=file://$ns_ior "
                           . " -x -c 1 -s 3 -o 30");

$S = new PerlACE::Process ("ECT_Supplier",
                           "-ORBInitRef NameService=file://$ns_ior "
                           . " -s 3 -u 10000 -n 1 -t 0 -b 8");
$S2 = new PerlACE::Process ("ECT_Supplier",
                           "-ORBInitRef NameService=file://$ns_ior "
                           . " -s 1 -u 10 -n 1 -t 0");

$NS->Spawn ();

if (PerlACE::waitforfile_timed ($ns_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
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

$consumer = $C->WaitKill (300);

# The consumer will probably fail to get all the messages due to the
#  EC dropping them.  We dont care if the Consumer fails.

$supplier2 = $S2->SpawnWaitKill (60);

if ($supplier2 != 0) {
    print STDERR "ERROR: second supplier returned $supplier2\n";
    $status = 1;
}


$eserver = $ES->TerminateWaitKill (10);

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
