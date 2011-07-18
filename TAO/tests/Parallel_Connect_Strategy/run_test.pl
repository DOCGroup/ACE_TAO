eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$iorbase = "altiiop.ior";
$status = 0;
@bogus_eps = ("-orbendpoint iiop://localhost:10200/hostname_in_ior=126.0.0.123",
              "-orbendpoint iiop://localhost:10202/hostname_in_ior=126.0.0.124");

if (PerlACE::is_vxworks_test()) {
  $iorfile = $iorbase;
  $valid_ep = "-orbendpoint iiop://".$ENV{'ACE_RUN_VX_TGTHOST'}.":10201";
  $corbaloc = "corbaloc::126.0.0.123:10200,:".$ENV{'ACE_RUN_VX_TGTHOST'}.":10201,:126.0.0.124:10202/pcs_test";
  $SV_ALT_IIOP = new PerlACE::ProcessVX ("server", "-ORBUseSharedProfile 1 -o $iorfile $bogus_eps[0] $valid_ep $bogus_eps[1]");
}
else {
  $valid_ep = "-orbendpoint iiop://localhost:10201";
  $iorfile = PerlACE::LocalFile ($iorbase);
  $corbaloc = "corbaloc::126.0.0.123:10200,:localhost:10201,:126.0.0.124:10202/pcs_test";
  $SV_ALT_IIOP = new PerlACE::Process ("server", "-ORBUseSharedProfile 1 -o $iorfile $bogus_eps[0] $valid_ep $bogus_eps[1]");
}
unlink $iorfile;

$CL_LF = new PerlACE::Process ("client", "-ORBuseParallelConnects 1 -k file://$iorfile");
$CL_CORBALOC = new PerlACE::Process ("client", "-ORBUseSharedProfile 1 -ORBuseParallelConnects 1 -k $corbaloc");
$CL_Reactive = new PerlACE::Process ("client", "-ORBSvcConf reactive.conf -ORBuseParallelConnects 1 -k file://$iorfile");
$CL_Blocked = new PerlACE::Process ("client", "-ORBSvcConf blocked.conf -ORBuseParallelConnects 1 -k file://$iorfile");
$CL_None = new PerlACE::Process ("client", "-ORBuseParallelConnects 0 -k file://$iorfile");
if ($ARGV[0] eq '-quick')  {
  $CL_Blocked = new PerlACE::Process ("client", "-ORBSvcConf blocked.conf -ORBuseParallelConnects 1 -k file://$iorfile -t");
  $CL_None = new PerlACE::Process ("client", "-ORBuseParallelConnects 0 -k file://$iorfile -t");
}
$CL_Shutdown =  new PerlACE::Process ("client", "-ORBuseParallelConnects 1 -k file://$iorfile -x");

$SV_ALT_IIOP->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV_ALT_IIOP->Kill ();
    $SV_ALT_IIOP->TimedWait (1);
    exit 1;
}

print "LF wait strategy test\n";

$client = $CL_LF->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

print "\nLF wait strategy, corbaloc test\n";

$client = $CL_CORBALOC->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

print "\nReactive wait strategy test\n";

$client = $CL_Reactive->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

print "\nBlocked wait strategy test\n";

$client = $CL_Blocked->SpawnWaitKill (600);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

print "\nNo parallel connect test\n";

$client = $CL_None->SpawnWaitKill (900);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$client = $CL_Shutdown->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV_ALT_IIOP->WaitKill (60);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

exit $status;
