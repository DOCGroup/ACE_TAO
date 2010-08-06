# $Id$ 

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use Env (ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::Run_Test;

$ENV{'SSL_CERT_FILE'} = 'cacert.pem';

$file = PerlACE::LocalFile("Messenger.ior");

unlink $file;

# start MessengerServer
$S = new PerlACE::Process("MessengerServer",
			  "-ORBSvcConf server.conf");
$S->Spawn();
if (PerlACE::waitforfile_timed ($file, 15) == -1) {
    print STDERR "ERROR: cannot find file <$file>\n";
    $S->Kill ();
    exit 1;
}

# start MessengerClient
$C = new PerlACE::Process("MessengerClient",
			  "-ORBSvcConf client.conf");

if ($C->SpawnWaitKill(10) != 0) {
     exit (1);
}

if ($S->WaitKill(10) == -1) {
     $S->Kill();
}

exit 0;

