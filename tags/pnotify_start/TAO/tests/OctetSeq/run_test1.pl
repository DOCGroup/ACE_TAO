eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;

$status = 0;
$iorfile = PerlACE::LocalFile ("test1.ior");

unlink $iorfile;

$SV = new PerlACE::Process ("server", "-ORBSvcConf svc1$PerlACE::svcconf_ext -o $iorfile");
$CL = new PerlACE::Process ("client", "-ORBSvcConf svc1$PerlACE::svcconf_ext -i 5000 -k file://$iorfile");
$T = new PerlACE::Process ("OctetSeq", "-ORBSvcConf svc1$PerlACE::svcconf_ext -n 32 -l 8192 -h 8192 -s 1 -q");

print STDERR "\n\n==== Octet sequence passing test\n";

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (120);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

print STDERR "\n\n==== Octet sequence performance test\n";

$test = $T->SpawnWaitKill (60);

if ($test != 0) {
    print STDERR "ERROR: test returned $test\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
