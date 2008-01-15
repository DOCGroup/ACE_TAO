eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$srv_threads = '4';
$cli_threads = '30';
$baseior = "test.ior";
$iorfile = PerlACE::LocalFile ($baseior);
$srvdownfile = PerlACE::LocalFile ("server_terminated");

unlink $iorfile;
unlink $srvdownfile;

if (PerlACE::is_vxworks_test()) {
  $SV = new PerlACE::ProcessVX ("server", "-o $baseior -n $srv_threads");
}
else {
  $SV = new PerlACE::Process ("server", "-o $iorfile -n $srv_threads");
}
$CL = new PerlACE::Process ("client", "-k file://$iorfile -n $cli_threads");
$CLS = new PerlACE::Process ("client", "-k file://$iorfile -x");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$CL->Spawn ();
#Client will not terminate on its own, kill it after 30 seconds
$CL->TimedWait (30);
$CL->Kill (1);
#The parameter '1' (above) indicates that an error return value should not
#cause this test to fail.

print "***Client has been killed***\n";

#Server should still be running at this point, now start the client that asks
#the server to shut itself down

$client = $CLS->SpawnWaitKill (15);
if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

#Server should now be shutdown
if (PerlACE::waitforfile_timed ($srvdownfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$srvdownfile>".
        " -- server has not shut down cleanly.\n";
    $SV->Kill ();
    exit 1;
}

$server = $SV->WaitKill (15);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;
unlink $srvdownfile;

exit $status
