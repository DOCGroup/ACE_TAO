eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../bin";
use PerlACE::Run_Test;

$status = 0;
$iorfile = PerlACE::LocalFile ("test.ior");
$svc_conf = PerlACE::LocalFile ("svc.conf");

print STDERR "================ Throughput test\n";

unlink $iorfile;

$SV = new PerlACE::Process ("server",
                            " -ORBSvcConf $svc_conf");

$CL = new PerlACE::Process ("client",
                            " -ORBSvcConf $svc_conf -ORBNoDelay 1"
                            . " -x");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); 
    exit 1;
}

$client = $CL->SpawnWaitKill (6000);
$server = $SV->WaitKill (10);

unlink $iorfile;

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

exit $status;
