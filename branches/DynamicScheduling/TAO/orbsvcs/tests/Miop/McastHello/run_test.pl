eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib '../../../../../bin';
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("server.ior");
unlink $iorfile;
$status = 0;

$server_conf = PerlACE::LocalFile ("server.conf");
$client_conf = PerlACE::LocalFile ("client.conf");

$SV1 = new PerlACE::Process ("server", "-o $iorfile -ORBsvcconf $server_conf");
$SV2 = new PerlACE::Process ("server", "-o $iorfile -ORBsvcconf $server_conf");
$CL = new PerlACE::Process ("client", " -k file://$iorfile -ORBsvcconf $client_conf");

$SV1->Spawn ();
$SV2->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
} 

$client = $CL->SpawnWaitKill (300);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV1->WaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

$server = $SV2->WaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
