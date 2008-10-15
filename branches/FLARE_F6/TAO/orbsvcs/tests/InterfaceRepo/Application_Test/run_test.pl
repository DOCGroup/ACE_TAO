eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

$ifr_iorfile= "if_repo.ior";
$svr_iorfile = "iorfile";
$test_idl = PerlACE::LocalFile ("test.idl");

$lookup_by_name = "";
$other = "";

for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-n") {
        $lookup_by_name = "-n";
    }
    else {
        $other .= $ARGV[$i];
    }
}

$TAO_IFR = new PerlACE::Process ("$ENV{ACE_ROOT}/bin/tao_ifr");

# We want the tao_ifr executable to be found exactly in the path
# given, without being modified by the value of -ExeSubDir.
# So, we tell its Process object to ignore the setting of -ExeSubDir.

$TAO_IFR->IgnoreExeSubDir (1);

$IFR     = new PerlACE::Process ("../../../IFR_Service/IFR_Service", " -o $ifr_iorfile");
$SV      = new PerlACE::Process ("server", "-ORBInitRef InterfaceRepository=file://$ifr_iorfile");
$CL      = new PerlACE::Process ("client", "-ORBInitRef InterfaceRepository=file://$ifr_iorfile"
                                 . " $lookup_by_name");
$CL2     = new PerlACE::Process ("client", "-ORBInitRef InterfaceRepository=file://$ifr_iorfile -n");

unlink $ifr_iorfile;
unlink $svr_iorfile;

$IFR->Spawn ();

if (PerlACE::waitforfile_timed ($ifr_iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$ifr_iorfile>\n";
    $IFR->Kill ();
    exit 1;
}

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($svr_iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$svr_iorfile>\n";
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

$TAO_IFR->Arguments ("-ORBInitRef InterfaceRepository=file://$ifr_iorfile -r $test_idl");

$tresult = $TAO_IFR->SpawnWaitKill (30);

if ($tresult != 0) {
    print STDERR "ERROR: tao_ifr (-r test.idl) returned $tresult\n";
    $status = 1;
}

# Do another lookup to check it really has been removed.
$client = $CL2->SpawnWaitKill (60);

if ($client == 0) {
  print STDERR "ERROR: second client run returned $client\n";
  $status = 1;
}

$server = $SV->TerminateWaitKill (5);

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
unlink $svr_iorfile;

exit $status;
