eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;

$status = 0;
$iorfile = PerlACE::LocalFile ("test.ior");

$svcfile = PerlACE::LocalFile ("svc.conf");

$SV = new PerlACE::Process ("./ami_server", "-ORBSvcConf $svcfile -o $iorfile");
$CL = new PerlACE::Process ("./ami_client", "-ORBSvcConf $svcfile -k file://$iorfile");

print STDERR "==== Server upcall waits for operations on other threads\n";

unlink $iorfile;

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 30) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $BSV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (240);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
