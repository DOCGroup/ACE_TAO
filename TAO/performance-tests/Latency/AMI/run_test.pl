eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../bin";
use PerlACE::Run_Test;

$status = 0;
$iorfile = PerlACE::LocalFile ("test.ior");

print STDERR "================ AMI Latency test\n";

unlink $iorfile;

$SV = new PerlACE::Process ("server",
                            "-o $iorfile");

$CL = new PerlACE::Process ("client",
                            "-k file://$iorfile");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (60);
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
