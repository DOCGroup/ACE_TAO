eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

$ifr_iorfile= "if_repo.ior";
$test_idl = PerlACE::LocalFile ("test.idl");
$includes = "-I ../../../..  -I ../../../../orbsvcs";

# find the tao_ifr executable.
# Its placement is dependent upon the OS and if MPC generated makefiles are used.
my $exec_extn="";
if ($^O eq "MSWin32") {
  $exec_extn=".exe";
}

$tao_ifr = "$ENV{ACE_ROOT}/bin/tao_ifr";
if (! -e $tao_ifr . $exec_extn ) {
    $tao_ifr = "../../../../orbsvcs/IFR_Service/tao_ifr";
        if (! -e $tao_ifr . $exec_extn ) {
            print STDERR "ERROR: tao_ifr compiler not found.\n";
            exit 1;
    }
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
$IFR     = new PerlACE::Process ("../../../../orbsvcs/IFR_Service/IFR_Service", " -o $ifr_iorfile");
$CL      = new PerlACE::Process ("IDL3_Test", "-ORBInitRef InterfaceRepository=file://$ifr_iorfile"
                                 . " $debug");

unlink $ifr_iorfile;

$IFR->Spawn ();

if (PerlACE::waitforfile_timed ($ifr_iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$ifr_iorfile>\n";
    $IFR->Kill ();
    exit 1;
}

$TAO_IFR->Arguments ("-ORBInitRef InterfaceRepository=file://$ifr_iorfile $includes $test_idl");

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
