eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../bin";
use PerlACE::Run_Test;

$status = 0;

$ifr_iorfile= "if_repo.ior";
$svr_iorfile = "iorfile";
$test_idl = PerlACE::LocalFile ("test.idl");

if ($^O eq "MSWin32") {
    $tao_ifr = "../../../../../bin/tao_ifr";
}
else {
    $tao_ifr = "../../../IFR_Service/tao_ifr";
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

$TAO_IFR = new PerlACE::Process ($tao_ifr);
$IFR     = new PerlACE::Process ("../../../IFR_Service/IFR_Service");
$SV      = new PerlACE::Process ("server");
$CL      = new PerlACE::Process ("client", "-ORBInitRef InterfaceRepository=file://$ifr_iorfile"
                                 . " $lookup_by_name");
$CL2     = new PerlACE::Process ("client", "-ORBInitRef InterfaceRepository=file://$ifr_iorfile -n");

unlink $ifr_iorfile;
unlink $svr_iorfile;

$IFR->Spawn ();

if (PerlACE::waitforfile_timed ($ifr_iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$ifr_iorfile>\n";
    $IFR->Kill ();
    exit 1;
}

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($svr_iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$svr_iorfile>\n";
    $IFR->Kill ();
    $SV->Kill ();
    exit 1;
}

$TAO_IFR->Arguments ($test_idl);

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

$TAO_IFR->Arguments ("-r $test_idl");

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
