eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("server.ior");
$orbport = 10000 + PerlACE::uniqueid ();
unlink $iorfile;
$status = 0;

if (PerlACE::is_vxworks_test()) {
$endpoint = "-ORBEndpoint iiop://".$ENV{'ACE_RUN_VX_TGTHOST'}.":$orbport";
$clientarg = "-h ".$ENV{'ACE_RUN_VX_TGTHOST'}." -p $orbport";
$SV = new PerlACE::ProcessVX ("server", "$endpoint");
}
else {
$endpoint = "-ORBEndpoint iiop://localhost:$orbport";
$clientarg = "-h localhost -p $orbport";
$SV = new PerlACE::Process ("server", "$endpoint");
}

$CL = new PerlACE::Process ("client", "$clientarg");

print STDERR $CL->CommandLine(), "\n" ;
$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client = $CL->SpawnWaitKill (300);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

print "Calling shutdown client\n";

$SHUTDOWN = new PerlACE::Process ("shutdown_client", "-k file://$iorfile");

$client = $SHUTDOWN->SpawnWaitKill (300);

if ($client != 0) {
    print STDERR "ERROR: shutdown_client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (120);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
