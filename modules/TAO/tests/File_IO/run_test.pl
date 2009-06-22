eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("server.ior");
$status = 0;
$server_debug_level = '0';
$client_debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-sdebug') {
        $server_debug_level = '10';
    } 
    if ($i eq '-cdebug') {
        $client_debug_level = '10';
    } 
}

unlink $iorfile;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "-ORBDebuglevel $server_debug_level -o server.ior");
}
else {
    $SV = new PerlACE::Process ("server", "-ORBDebuglevel $server_debug_level -o $iorfile");
}
$CL = new PerlACE::Process ("client", "-ORBDebuglevel $client_debug_level -k $iorfile");

$server = $SV->Spawn ();

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    exit 1;
}

if (PerlACE::waitforfile_timed ($iorfile, 
                                $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (120);
$server = $SV->TerminateWaitKill (10);

unlink $iorfile;
unlink "test"; # created by the program
unlink "big.txt"; # created by the program

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

exit $status;
