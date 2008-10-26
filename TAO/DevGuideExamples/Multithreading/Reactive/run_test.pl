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

$S = new PerlACE::Process("MessengerServer", "-ORBEndpoint iiop://localhost");
$S->Spawn();

if (PerlACE::waitforfile_timed ($ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$ior>\n";
    $S->Kill(); 
    unlink $ior;
    exit 1;
}

# start MessengerClient

$C = new PerlACE::Process("MessengerClient");  
$C->Spawn();

$CRET = $C->WaitKill(15);
$S->Kill();

# clean-up 

unlink $ior;

if ($CRET != 0) {
    print STDERR "ERROR: Client returned <$CRET>\n";
    exit 1 ;
}  

exit 0;



