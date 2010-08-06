eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

$ifr_iorfile = PerlACE::LocalFile ("if_repo.ior");
$test_idl = PerlACE::LocalFile ("Test.idl");
$iorfile = PerlACE::LocalFile ("server.ior");

unlink $ifr_iorfile;
unlink $iorfile;

$TAO_IFR = new PerlACE::Process ("$ENV{ACE_ROOT}/bin/tao_ifr");
$TAO_IFR->IgnoreExeSubDir (1);

$IFR     = new PerlACE::Process ("../../../IFR_Service/IFR_Service", " -o $ifr_iorfile");

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "-ORBInitRef InterfaceRepository=file://if_repo.ior -ORBDebuglevel $debug_level -o server.ior");
}
else {
    $SV = new PerlACE::Process ("server", "-ORBInitRef InterfaceRepository=file://$ifr_iorfile -ORBdebuglevel $debug_level -o $iorfile");
}
$CL = new PerlACE::Process ("client", "-ORBInitRef InterfaceRepository=file://$ifr_iorfile -k file://$iorfile");


$IFR->Spawn ();

if (PerlACE::waitforfile_timed ($ifr_iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$ifr_iorfile>\n";
    $IFR->Kill ();
    exit 1;
}

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $IFR->Kill ();
    $SV->Kill ();
    exit 1;
}

$TAO_IFR->Arguments ("-ORBInitRef InterfaceRepository=file://$ifr_iorfile $test_idl");

$tresult = $TAO_IFR->SpawnWaitKill (30);

if ($tresult != 0) {
    print STDERR "ERROR: tao_ifr (test.idl) returned $tresult\n";
    $status = 1;
}

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->TerminateWaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

$server = $IFR->TerminateWaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: IFR returned $server\n";
    $status = 1;
}

unlink $ifr_iorfile;
unlink $iorfile;

exit $status;
