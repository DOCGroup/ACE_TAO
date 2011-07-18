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

$S = new PerlACE::Process("MessengerServer", "-ORBEndpoint iiop://localhost");
$S->Spawn();

if (PerlACE::waitforfile_timed ($ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$ior>\n";
    $S->Kill(); 
    unlink $ior;
    exit 1;
}

# start several MessengerClients

print STDOUT "\n\nStarting 9 MessengerClients.\n";
print STDOUT "The server should use different threads to handle requests.\n\n";

$C1 = new PerlACE::Process("MessengerClient");  
$C2 = new PerlACE::Process("MessengerClient");  
$C3 = new PerlACE::Process("MessengerClient");  
$C4 = new PerlACE::Process("MessengerClient");  
$C5 = new PerlACE::Process("MessengerClient");  
$C6 = new PerlACE::Process("MessengerClient");  
$C7 = new PerlACE::Process("MessengerClient");  
$C8 = new PerlACE::Process("MessengerClient");  
$C9 = new PerlACE::Process("MessengerClient");  
$C1->Spawn();
$C2->Spawn();
$C3->Spawn();
$C4->Spawn();
$C5->Spawn();
$C6->Spawn();
$C7->Spawn();
$C8->Spawn();
$C9->Spawn();

$C1RET = $C1->WaitKill(15);
$C2RET = $C2->WaitKill(15);
$C3RET = $C3->WaitKill(15);
$C4RET = $C4->WaitKill(15);
$C5RET = $C5->WaitKill(15);
$C6RET = $C6->WaitKill(15);
$C7RET = $C7->WaitKill(15);
$C8RET = $C8->WaitKill(15);
$C9RET = $C9->WaitKill(15);
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

if ($C5RET != 0) {
    print STDERR "ERROR: Client 1 returned <$C5RET>\n";
    exit 1 ;
}  

if ($C6RET != 0) {
    print STDERR "ERROR: Client 1 returned <$C6RET>\n";
    exit 1 ;
}  

if ($C7RET != 0) {
    print STDERR "ERROR: Client 1 returned <$C7RET>\n";
    exit 1 ;
}  

if ($C8RET != 0) {
    print STDERR "ERROR: Client 1 returned <$C8RET>\n";
    exit 1 ;
}  

if ($C9RET != 0) {
    print STDERR "ERROR: Client 1 returned <$C9RET>\n";
    exit 1 ;
}  

exit 0;



