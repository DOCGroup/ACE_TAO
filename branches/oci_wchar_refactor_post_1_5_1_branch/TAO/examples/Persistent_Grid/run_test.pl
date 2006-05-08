eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../bin";
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("grid.ior");
$mem     = PerlACE::LocalFile ("foo");
$status  = 0;

$SV  = new PerlACE::Process ("server", "-o $iorfile -m $mem");
$CL  = new PerlACE::Process ("client", "-k file://$iorfile");
$PCL = new PerlACE::Process ("persistent_client", "-k file://$iorfile -w 10 -h 10");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 10) == -1) {
    print STDERR "ERROR: Could not find file <$iorfile>\n";
    $SV->Kill (); 
    exit 1;
}

$client = $CL->SpawnWaitKill (60);

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

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 10) == -1) {
    print STDERR "ERROR: Could not find file <$iorfile>\n";
    $server->Kill (); 
    exit 1;
}

$pclient = $PCL->SpawnWaitKill (60);

if ($pclient != 0) {
    print STDERR "ERROR: persistent client returned $pclient\n";
    $status = 1;
}

$server = $SV->TerminateWaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;
unlink $mem;

exit $status;
