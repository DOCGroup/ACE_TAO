eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;

$status = 0;
$threads = '8';
$iorfile = PerlACE::LocalFile ("test.ior");
$sv_conf = PerlACE::LocalFile ("server$PerlACE::svcconf_ext");

unlink $iorfile;

$SV = new PerlACE::Process ("server", "-ORBsvcconf $sv_conf -o $iorfile -n $threads");
$CL1 = new PerlACE::Process ("client", "-k file://$iorfile -i 100");
$CL2 = new PerlACE::Process ("client", "-k file://$iorfile -i 100");
$CL3 = new PerlACE::Process ("client", "-k file://$iorfile -i 100");
$CLS = new PerlACE::Process ("client", "-k file://$iorfile -i 100 -x ");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$CL1->Spawn ();
$CL2->Spawn ();
$CL3->Spawn ();

$client = $CL1->WaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}
$client = $CL2->WaitKill (30);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$client = $CL3->WaitKill (30);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$client = $CLS->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status
