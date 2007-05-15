eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$iorfile = PerlACE::LocalFile ("test.ior");

unlink $iorfile;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "-ORBsvcconf server$PerlACE::svcconf_ext -o test.ior -c 3 -i 100");
}
else {
    $SV = new PerlACE::Process ("server", "-ORBSvcConf server$PerlACE::svcconf_ext -o $iorfile -c 3 -i 100");
}
$CL1 = new PerlACE::Process ("client", "-k file://$iorfile -ORBDottedDecimalAddresses 1");
$CL2 = new PerlACE::Process ("client", "-k file://$iorfile -ORBDottedDecimalAddresses 1");
$CL3 = new PerlACE::Process ("client", "-k file://$iorfile -ORBDottedDecimalAddresses 1");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$CL1->Spawn (60);
$CL2->Spawn (60);
$CL3->Spawn (60);

$client1 = $CL1->WaitKill (75);

if ($client1 != 0) {
    print STDERR "ERROR: client 1 returned $client1\n";
    $status = 1;
}

$client2 = $CL2->WaitKill (75);

if ($client2 != 0) {
    print STDERR "ERROR: client 2 returned $client2\n";
    $status = 1;
}

$client3 = $CL3->WaitKill (75);

if ($client3 != 0) {
    print STDERR "ERROR: client 3 returned $client3\n";
    $status = 1;
}

$server = $SV->WaitKill (20);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
