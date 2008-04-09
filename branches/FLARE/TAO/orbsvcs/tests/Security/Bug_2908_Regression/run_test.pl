# $Id$
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use Env (ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::Run_Test;

$ENV{'SSL_CERT_FILE'} = 'cacert.pem';
$iorfile = "server.ior";
unlink $iorfile;

# start MessengerServer
$SV = new PerlACE::Process("server", "-ORBSvcConf server.conf");
$SV->Spawn();
if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}


# start MessengerClient
$CL = new PerlACE::Process("client", "-ORBSvcConf client.conf");

if ($CL->SpawnWaitKill(10) == -1) {
     exit (1);
}

$SV->WaitKill(15);

unlink $iorfile;

exit 0;

