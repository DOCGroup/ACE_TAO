eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

$ifr_iorfile= "if_repo.ior";
$srv_iorfile = "iorfile.ior";
$gateway_iorfile = "gateway_ior.ior";
$test_idl = PerlACE::LocalFile ("sum_server.idl");

# find the tao_ifr executable.
# Its placement is dependent upon the OS and if MPC generated makefiles are used.
my $exec_extn="";
if ($^O eq "MSWin32") {
  $exec_extn=".exe";
}

$tao_ifr = "../../../../bin/tao_ifr";
if (! -e $tao_ifr . $exec_extn ) {
    $tao_ifr = "../../../IFR_Service/tao_ifr";
    if (! -e $tao_ifr . $exec_extn ) {
        print STDERR "ERROR: tao_ifr compiler not found.\n";
        exit 1;
    }
}

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

$TAO_IFR   = new PerlACE::Process ($tao_ifr);
$IFR       = new PerlACE::Process ("../../IFR_Service/IFR_Service", " -o $ifr_iorfile");
$GATEWAYSV = new PerlACE::Process ("gateway_server", "-o $gateway_iorfile -ORBInitRef IFR_Service=file://$ifr_iorfile");
$SV        = new PerlACE::Process ("server", "-o $srv_iorfile -ORBInitRef Gateway_Object_Factory=file://$gateway_iorfile");
$CL2       = new PerlACE::Process ("client", "-k file://$srv_iorfile");

unlink $ifr_iorfile;
unlink $svr_iorfile;
unlink $gateway_iorfile;

$IFR->Spawn ();

if (PerlACE::waitforfile_timed ($ifr_iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$ifr_iorfile>\n";
    $IFR->Kill ();
    exit 1;
}

$TAO_IFR->Arguments ("-ORBInitRef InterfaceRepository=file://$ifr_iorfile $test_idl");

$tresult = $TAO_IFR->SpawnWaitKill (30);

$GATEWAYSV->Spawn ();

if (PerlACE::waitforfile_timed ($gateway_iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$gateway_iorfile>\n";
    $IFR->Kill ();
    $GATEWAYSV->Kill ();
    exit 1;
}

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($svr_iorfile, 1500) == -1) {
    print STDERR "ERROR: cannot find file <$srv_iorfile>\n";
    $IFR->Kill ();
    $GATEWAYSV->Kill ();
    $SV->Kill ();
    exit 1;
}

if ($tresult != 0) {
    print STDERR "ERROR: tao_ifr (test.idl) returned $tresult\n";
    $status = 1;
}

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$tresult = $TAO_IFR->SpawnWaitKill (30);

if ($tresult != 0) {
    print STDERR "ERROR: tao_ifr (-r test.idl) returned $tresult\n";
    $status = 1;
}

$gatewayserver = $GATEWAYSV->TerminateWaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
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
unlink $srv_iorfile;
unlink $gateway_iorfile;

exit $status;
