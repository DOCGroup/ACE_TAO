eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;

$status = 0;
$iorfile = PerlACE::LocalFile ("test.ior");
$svcconf = PerlACE::LocalFile ("bala$PerlACE::svcconf_ext");

unlink $iorfile;

$SV = new PerlACE::Process ("server", "-ORBSvcConf server$PerlACE::svcconf_ext -o $iorfile -c 3 -i 100");
$CL1 = new PerlACE::Process ("client", "-k file://$iorfile");
$CL2 = new PerlACE::Process ("client", "-k file://$iorfile");
$CL3 = new PerlACE::Process ("client", "-k file://$iorfile");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 15) == -1) {
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
