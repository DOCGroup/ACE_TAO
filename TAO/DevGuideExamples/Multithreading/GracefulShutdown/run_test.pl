# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use Env (ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::Run_Test;

$ior = PerlACE::LocalFile ("Messenger.ior");
unlink $ior;

$server_args = "-ORBEndpoint iiop://localhost";

# -------------------------------------------------------------------
# Test 1: Shutdown on client invocation
# -------------------------------------------------------------------

# start MessengerServer

print STDOUT "\n\nTest 1: Shutdown on client invocation.\n";
print STDOUT "Running MessengerServer...\n";
$S1 = new PerlACE::Process("MessengerServer", $server_args . " -x");
$S1->Spawn();

if (PerlACE::waitforfile_timed ($ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$ior>\n";
    $S1->Kill(); 
    unlink $ior;
    exit 1;
}

# start MessengerClient

$C1 = new PerlACE::Process("MessengerClient", "-x");  
$C1->Spawn();

$C1RET = $C1->WaitKill(15);
$S1->Kill();

# clean-up 

unlink $ior;

if ($C1RET != 0) {
    print STDERR "ERROR: Client returned <$C1RET>\n";
    exit 1 ;
}  

# -------------------------------------------------------------------
# Test 2: Shutdown after <n> iterations through polling loop
# -------------------------------------------------------------------

# start MessengerServer

$iter = 10;
print STDOUT "\n\nTest 2: Shutdown after <$iter> iterations through polling loop.\n";
print STDOUT "Running MessengerServer...\n";
$S2 = new PerlACE::Process("MessengerServer", $server_args . " -p " . $iter);
$S2->Spawn();

if (PerlACE::waitforfile_timed ($ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$ior>\n";
    $S2->Kill(); 
    unlink $ior;
    exit 1;
}

# start MessengerClient

$C2 = new PerlACE::Process("MessengerClient");  
$C2->Spawn();

$C2RET = $C2->WaitKill(15);
$S2->WaitKill($iter+5);

# clean-up 

unlink $ior;

if ($C2RET != 0) {
    print STDERR "ERROR: Client returned <$C2RET>\n";
    exit 1 ;
}  

# -------------------------------------------------------------------
# Test 3: Schedule a timer with the ORB's reactor to shutdown 
#         in <n> seconds
# -------------------------------------------------------------------

# start MessengerServer

$sec = 10;
print STDOUT "\n\nTest 3: Schedule a timer with the ORB's reactor to shutdown in <$sec> seconds.\n";
print STDOUT "Running MessengerServer...\n";
$S3 = new PerlACE::Process("MessengerServer", $server_args . " -t " . $sec);
$S3->Spawn();

if (PerlACE::waitforfile_timed ($ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$ior>\n";
    $S3->Kill(); 
    unlink $ior;
    exit 1;
}

# start MessengerClient

$C3 = new PerlACE::Process("MessengerClient");  
$C3->Spawn();

$C3RET = $C3->WaitKill(15);
$S3->WaitKill($sec+5);

# clean-up 

unlink $ior;

if ($C3RET != 0) {
    print STDERR "ERROR: Client returned <$C3RET>\n";
    exit 1 ;
}  

# -------------------------------------------------------------------
# Test 4: Use the overloaded version of CORBA::ORB::run() that takes
#         an ACE_Time_Value parameter indicating how long run()
#         should process events before returning.
# -------------------------------------------------------------------

# start MessengerServer

print STDOUT "\n\nTest 4: Use the overloaded version of CORBA::ORB::run()\n";
print STDOUT "that takes an ACE_Time_Value parameter indicating how long\n";
print STDOUT "run() should process events before returning (<$sec> seconds).\n";
print STDOUT "Running MessengerServer...\n";
$S4 = new PerlACE::Process("MessengerServer", $server_args . " -r " . $sec);
$S4->Spawn();

if (PerlACE::waitforfile_timed ($ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$ior>\n";
    $S4->Kill(); 
    unlink $ior;
    exit 1;
}

# start MessengerClient

$C4 = new PerlACE::Process("MessengerClient");  
$C4->Spawn();

$C4RET = $C4->WaitKill(15);
$S4->WaitKill($sec+5);

# clean-up 

unlink $ior;

if ($C4RET != 0) {
    print STDERR "ERROR: Client returned <$C4RET>\n";
    exit 1 ;
}  


# -------------------------------------------------------------------
# Test 5: Spawn a separate thread to shutdown the ORB on any
#         input from the console (terminal)
# -------------------------------------------------------------------

# start MessengerServer

print STDOUT "\n\nTest 5: Spawn a separate thread to shutdown the ORB on any input from the console (terminal).\n";
print STDOUT "Running MessengerServer...\n";
$S5 = new PerlACE::Process("MessengerServer", $server_args . " -c");
$S5->Spawn();

if (PerlACE::waitforfile_timed ($ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$ior>\n";
    $S5->Kill(); 
    unlink $ior;
    exit 1;
}

# start MessengerClient

$C5 = new PerlACE::Process("MessengerClient");  
$C5->Spawn();

$C5RET = $C5->WaitKill(15);
print STDOUT "Enter any input to shutdown MessengerServer...\n";
$S5->WaitKill(15);

# clean-up 

unlink $ior;

if ($C5RET != 0) {
    print STDERR "ERROR: Client returned <$C5RET>\n";
    exit 1 ;
}  


exit 0;



