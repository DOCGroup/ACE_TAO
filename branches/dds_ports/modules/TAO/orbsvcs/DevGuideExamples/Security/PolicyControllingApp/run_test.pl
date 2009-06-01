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

$S1 = new PerlACE::Process("MessengerServer",
                          "-ORBSvcConf server1.conf");

$C = new PerlACE::Process("MessengerClient",
			  "-e 1 -ORBSvcConf client.conf");

$C1 = new PerlACE::Process("MessengerClient",
                           "-e 2 -ORBSvcConf client1.conf");


print STDERR "\n\nSecurity Policy Controlling Application Examples\n";
print STDERR     "------------------------------------------------\n";

print STDERR     "Starting Messenger Server, example 1...\n\n";
$S->Spawn();
if (PerlACE::waitforfile_timed ($file, 5) == -1) {
    print STDERR "ERROR: cannot find file <$file>\n";
    $SV->Kill ();
    exit 1;
}

print STDERR     "\nStarting MessengerClient, example 1...\n\n";
if ($C->SpawnWaitKill(10) != 0) {
     $S->Kill();
     exit (1);
}

unlink $file;
$S->Kill();

print STDERR     "\nStarting Messenger Server, example 2...\n\n";
$S1->Spawn();
if (PerlACE::waitforfile_timed ($file, 5) == -1) {
    print STDERR "ERROR: cannot find file <$file>\n";
    $SV->Kill ();
    exit 1;
}

print STDERR     "\nStarting MessengerClient, example 2...\n\n";
if ($C1->SpawnWaitKill(10) != 0) {
     $S->Kill();
     exit (1);
}

# clean-up 
$S1->Kill();

exit 0;

