eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../bin";
use PerlACE::Run_Test;

$status = 0;
$iorfile = "ior";
$conf = PerlACE::LocalFile ("client.conf");

# Remove the file before starting the test.
unlink $iorfile;

$SV = new PerlACE::Process ("server");
$CL = new PerlACE::Process ("client", "-f $iorfile -ORBSvcConf $conf");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 10) == -1) {
    print STDERR "ERROR: waiting for file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$SV->Kill (); 

unlink $iorfile;

exit $status;
