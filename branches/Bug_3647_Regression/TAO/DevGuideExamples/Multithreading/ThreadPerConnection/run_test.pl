# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use Env (ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::Run_Test;

$ior = PerlACE::LocalFile ("Messenger.ior");
unlink $ior;

# start MessengerServer

print STDOUT "Starting MessengerServer\n";

$S = new PerlACE::Process("MessengerServer", "-ORBSvcConf server.conf -ORBEndpoint iiop://localhost");
$S->Spawn();

if (PerlACE::waitforfile_timed ($ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$ior>\n";
    $S->Kill(); 
    unlink $ior;
    exit 1;
}

# start several MessengerClients

print STDOUT "\n\nStarting 4 MessengerClients.\n";
print STDOUT "Each client should get a new connection \
and its own thread in the server.\n\n";

$C1 = new PerlACE::Process("MessengerClient");  
$C2 = new PerlACE::Process("MessengerClient");  
$C3 = new PerlACE::Process("MessengerClient");  
$C4 = new PerlACE::Process("MessengerClient");  
$C1->Spawn();
$C2->Spawn();
$C3->Spawn();
$C4->Spawn();

$C1RET = $C1->WaitKill(15);
$C2RET = $C2->WaitKill(15);
$C3RET = $C3->WaitKill(15);
$C4RET = $C4->WaitKill(15);
$S->Kill();

# clean-up 

unlink $ior;

if ($C1RET != 0) {
    print STDERR "ERROR: Client 1 returned <$C1RET>\n";
    exit 1 ;
}  

if ($C2RET != 0) {
    print STDERR "ERROR: Client 1 returned <$C2RET>\n";
    exit 1 ;
}  

if ($C3RET != 0) {
    print STDERR "ERROR: Client 1 returned <$C3RET>\n";
    exit 1 ;
}  

if ($C4RET != 0) {
    print STDERR "ERROR: Client 1 returned <$C4RET>\n";
    exit 1 ;
}  

exit 0;



