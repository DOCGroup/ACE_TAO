eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$baseior = "test.ior";
$iorfile = PerlACE::LocalFile ($baseior);

unlink $iorfile;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "-f $baseior -ORBDottedDecimalAddresses 1");
    $CL = new PerlACE::Process ("client", "-k file://$iorfile -x -ORBDottedDecimalAddresses 1");
}
else {
    $SV = new PerlACE::Process ("server", "-f $iorfile");
    $CL = new PerlACE::Process ("client", "-k file://$iorfile -x");
}

print STDERR "\nrunning ST version of the client and the server\n\n";

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

if (PerlACE::is_vxworks_test()) {
    $SV->Arguments ("-f $baseior -t 3");
}
else {
    $SV->Arguments ("-f $iorfile -t 3");
}
$CL->Arguments ("-k file://$iorfile -x -n 10");

print STDERR "\nrunning MT version of the client and the server\n\n";

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
