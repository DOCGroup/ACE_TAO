# $Id$ 

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use Env (ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::Run_Test;

$file = PerlACE::LocalFile("Messenger.ior");

unlink $file;

$ENV{'SSL_CERT_FILE'} = 'cacert.pem';

# start MessengerServer
$S = new PerlACE::Process("MessengerServer",
                          "-ORBSvcConf server.conf");

print STDERR "\n\nSecurity Unaware Application Examples\n";
print STDERR     "-------------------------------------\n";
print STDERR     "Starting Messenger Server...\n\n";

$S->Spawn();
if (PerlACE::waitforfile_timed ($file, 5) == -1) {
    print STDERR "ERROR: cannot find file <$file>\n";
    $SV->Kill ();
    exit 1;
}

# start MessengerClient
$C = new PerlACE::Process("MessengerClient");

$C1 = new PerlACE::Process("MessengerClient",
                           "-ORBSvcConf client.conf");

$C2 = new PerlACE::Process("MessengerClient",
                           "-ORBSvcConf client1.conf");

print STDERR     "\nStarting MessengerClient, example 1...\n\n";
if ($C->SpawnWaitKill(10) == 0) {
     $S->Kill();
     exit (1);
}

print STDERR     "\nStarting MessengerClient, example 2...\n\n";
if ($C1->SpawnWaitKill(10) != 0) {
     $S->Kill();
     exit (1);
}

$ENV{'SSL_CERT_FILE'} = '';

print STDERR     "\nStarting MessengerClient, example 3...\n\n";
if ($C2->SpawnWaitKill(10) != 0) {
     $S->Kill();
     exit (1);
}

# clean-up
$S->Kill();

exit 0;
