eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$locking = "";

$iorfile = "if_repo.ior";

$status = 0;

$nice = "";

unlink $iorfile;

$SV = new PerlACE::Process ("../../../IFR_Service/IFR_Service",
                            " $nice " . " -o $iorfile" . " $locking");
$CL1 = new PerlACE::Process ("$ENV{ACE_ROOT}/bin/tao_ifr",
                             "-ORBInitRef InterfaceRepository=file://$iorfile -T test1.idl");
$CL2 = new PerlACE::Process ("$ENV{ACE_ROOT}/bin/tao_ifr",
                             "-ORBInitRef InterfaceRepository=file://$iorfile -T test2.idl");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client1 = $CL1->SpawnWaitKill (60);

if ($client1 != 0) {
    print STDERR "ERROR: client returned $client1\n";
    $status = 1;
}

$client2 = $CL2->SpawnWaitKill (60);

if ($client2 != 0) {
    print STDERR "ERROR: client returned $client2\n";
    $status = 1;
}

$server = $SV->TerminateWaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$locking = "";

$iorfile = "if_repo.ior";

$status = 0;

$nice = "";

$ifrflags = '-Si';

unlink $iorfile;

$SV = new PerlACE::Process ("../../../IFR_Service/IFR_Service", " $nice " . " -o $iorfile" . " $locking");
$CL1 = new PerlACE::Process ("$ENV{ACE_ROOT}/bin/tao_ifr",
                             "-ORBInitRef InterfaceRepository=file://$iorfile  $ifrflags test1.idl");
$CL2 = new PerlACE::Process ("$ENV{ACE_ROOT}/bin/tao_ifr",
                             "-ORBInitRef InterfaceRepository=file://$iorfile  $ifrflags test2.idl");

$CL3 = new PerlACE::Process ("$ENV{ACE_ROOT}/bin/tao_ifr",
                             "-ORBInitRef InterfaceRepository=file://$iorfile  $ifrflags test3.idl");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

foreach $i (1..2) {
$client1 = $CL1->SpawnWaitKill (60);

if ($client1 != 0) {
    print STDERR "ERROR: client returned $client1\n";
    $status = 1;
}

$client2 = $CL2->SpawnWaitKill (60);

if ($client2 != 0) {
    print STDERR "ERROR: client returned $client2\n";
    $status = 1;
}

$client3 = $CL3->SpawnWaitKill (60);

if ($client3 != 0) {
    print STDERR "ERROR: client returned $client3\n";
    $status = 1;
}
}

$server = $SV->TerminateWaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;

