eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$client_conf = PerlACE::LocalFile ("client.global$PerlACE::svcconf_ext");
$server_conf = PerlACE::LocalFile ("server$PerlACE::svcconf_ext");
$client_process = "client";
$debug_level = '0';
$threads = '4';
$status = 0;

foreach $i (@ARGV) {
    if ($i eq '-tss') {
        $client_conf = PerlACE::LocalFile ("client.tss$PerlACE::svcconf_ext");
    } elsif ($i eq '-debug') {
        $debug_level = '1';
    } elsif ($i eq '-creation') {
        $client_process = 'orb_creation';
    }
}

$iorfile = PerlACE::LocalFile ("server.ior");

unlink $iorfile;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "-ORBsvcconf server$PerlACE::svcconf_ext -ORBdebuglevel $debug_level -o server.ior");
}
else {
    $SV = new PerlACE::Process ("server", "-ORBsvcconf $server_conf -ORBdebuglevel $debug_level -o $iorfile");
}
$CL = new PerlACE::Process ($client_process,
                            "-ORBdebuglevel $debug_level "
                            . "-k file://$iorfile "
                            . "-n $threads -i 1000 -x");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (240);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (30);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}


unlink $iorfile;

exit $status;
