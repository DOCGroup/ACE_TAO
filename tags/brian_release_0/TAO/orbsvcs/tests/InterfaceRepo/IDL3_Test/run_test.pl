eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../bin";
use PerlACE::Run_Test;

$status = 0;

$ifr_iorfile= "if_repo.ior";
$test_idl = PerlACE::LocalFile ("test.idl");
 
if ($^O eq "MSWin32") {
    $tao_ifr = "../../../../../bin/tao_ifr";
}
else {
    $tao_ifr = "../../../IFR_Service/tao_ifr";
}

for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-d") {
        $debug = "-d";
    }
    else {
        $other .= $ARGV[$i];
    }
}

$TAO_IFR = new PerlACE::Process ($tao_ifr);
$IFR     = new PerlACE::Process ("../../../IFR_Service/IFR_Service");
$CL      = new PerlACE::Process ("IDL3_Test", "-ORBInitRef InterfaceRepository=file://$ifr_iorfile"
                                 . " $debug");

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
    $status = 1;
}

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $IFR->TerminateWaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: IFR returned $server\n";
    $status = 1;
}

unlink $ifr_iorfile;

exit $status;
