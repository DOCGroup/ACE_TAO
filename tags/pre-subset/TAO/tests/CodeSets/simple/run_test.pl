eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("server.ior");
unlink $iorfile;
$status = 0;

print STDOUT "Client using char translator\n\n";

$SV = new PerlACE::Process ("server", " -ORBDottedDecimalAddresses 1");
$CL = new PerlACE::Process ("client", " -ORBSvcConf cs_test.conf");

$SV->Spawn ();

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

$server = $SV->TerminateWaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

print STDOUT "\nServer using char translator\n\n";

$SV2 = new PerlACE::Process ("server", " -ORBDottedDecimalAddresses 1 -ORBSvcConf cs_test.conf");
$CL2 = new PerlACE::Process ("client");

$SV2->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
} 

$client2 = $CL2->SpawnWaitKill (300);

if ($client2 != 0) {
    print STDERR "ERROR: client returned $client2\n";
    $status = 1;
}

$server2 = $SV2->TerminateWaitKill (5);

if ($server2 != 0) {
    print STDERR "ERROR: server returned $server2\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
