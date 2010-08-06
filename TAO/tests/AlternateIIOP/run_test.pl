eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$iorbase = "altiiop.ior";
$iorfile = PerlACE::LocalFile ("altiiop.ior");
unlink $iorfile;
$status = 0;
@bogus_eps = ("-orbendpoint iiop://localhost:10210/hostname_in_ior=bogus.com",
              "-orbendpoint iiop://localhost:10212/hostname_in_ior=bogus.com");
$valid_ep = "-orbendpoint iiop://localhost:10211";

if (PerlACE::is_vxworks_test()) {
$SV_ALT_IIOP = new PerlACE::ProcessVX ("../Hello/server", "-o $iorbase -ORBUseSharedProfile 1 ",
                                       "$bogus_eps[0] $valid_ep $bogus_eps[1]");
}
else {
$SV_ALT_IIOP = new PerlACE::Process ("../Hello/server", "-o $iorfile -ORBUseSharedProfile 1 ",
                                     "$bogus_eps[0] $valid_ep $bogus_eps[1]");
}
$CL_ALT_IIOP = new PerlACE::Process ("../Hello/client", " -k file://$iorfile");
$SV_ALT_IIOP->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV_ALT_IIOP->Kill ();
    $SV_ALT_IIOP->TimedWait (1);
    exit 1;
}

$client = $CL_ALT_IIOP->SpawnWaitKill (300);

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
