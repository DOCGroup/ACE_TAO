# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use Env (ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::Run_Test;

$messenger_ior = "Messenger.ior";

unlink $messenger_ior;

# start MessengerServer
$S = new PerlACE::Process("MessengerServer");
$S->Spawn();
if (PerlACE::waitforfile_timed ($messenger_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$implrepo_ior>\n";
    $S->Kill(); 
    exit 1;
}
 
# start MessengerClient
$C = new PerlACE::Process("MessengerClient");  

if ($C->SpawnWaitKill(15) != 0) {
   print STDERR "ERROR: client failed\n";
   $S->Kill();
   exit 1;
}

$S->Kill();

unlink $messenger_ior;

exit 0;
