# $Id$ 

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use Env (ACE_ROOT);
use Env (TAO_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::Run_Test;

$status = 0;

$ifr_iorfile= "if_repo.ior";

$ifr_server = "$ENV{TAO_ROOT}/orbsvcs/IFR_Service/IFR_Service";
$tao_ifr = "$ENV{ACE_ROOT}/bin/tao_ifr";
$test_idl = "test.idl";

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

$TAO_IFR = new PerlACE::Process ($tao_ifr);
$IFR     = new PerlACE::Process ($ifr_server);
$CL      = new PerlACE::Process ("IFRBrowser", "-ORBInitRef InterfaceRepository=file://$ifr_iorfile $lookup_by_name");

# We want the tao_ifr executable to be found exactly in the path
# given, without being modified by the value of -ExeSubDir.
# So, we tell its Process object to ignore the setting of -ExeSubDir.

$TAO_IFR->IgnoreExeSubDir (1);

unlink $ifr_iorfile;

$IFR->Spawn ();

if (PerlACE::waitforfile_timed ($ifr_iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$ifr_iorfile>\n";
    $IFR->Kill ();
    exit 1;
}

$TAO_IFR->Arguments ("-ORBInitRef InterfaceRepository=file://$ifr_iorfile $test_idl");

$tresult = $TAO_IFR->SpawnWaitKill (30);

if ($tresult != 0) {
    print STDERR "ERROR: tao_ifr (test.idl) returned $tresult\n";
    exit 1;
}

$client = $CL->SpawnWaitKill (5);
if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

# remove the interface from the Interface Repository.
$TAO_IFR->Arguments ("-ORBInitRef InterfaceRepository=file://$ifr_iorfile -r $test_idl");

$tresult = $TAO_IFR->SpawnWaitKill (30);

if ($tresult != 0) {
    print STDERR "ERROR: tao_ifr (-r test.idl) returned $tresult\n";
    $status = 1;
}

$server = $IFR->TerminateWaitKill (5);
if ($server != 0) {
    print STDERR "ERROR: IFR returned $server\n";
    $status = 1;
}

unlink $ifr_iorfile;

exit $status;

