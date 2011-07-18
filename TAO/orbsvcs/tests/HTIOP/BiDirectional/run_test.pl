eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use Sys::Hostname;

$status = 0;
$iorfile = PerlACE::LocalFile ("test.ior");
$host = hostname();

unlink $iorfile;

$SV = new PerlACE::Process ("server", "-orbendpoint htiop://$host: -orbsvcconf outside.conf -o $iorfile -i 100");
$CL = new PerlACE::Process ("client", "-orbsvcconf inside.conf -k file://$iorfile");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (20);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (20);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
