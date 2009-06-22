# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use Env (ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::Run_Test;

$implrepo_server = "$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/ImplRepo_Service";
$imr_activator = "$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/ImR_Activator";
$tao_imr = "$ENV{ACE_ROOT}/bin/tao_imr";

# Note : We don't actually use SVR, but we need a way to get the 
#        path to the -ExeSubDir
my $SVR = new PerlACE::Process ('MessengerServer');
my $server = $SVR->Executable ();

# The Tests
$implrepo_ior = "implrepo.ior";
$activator_ior = "activator.ior";
$messenger_ior = "Messenger.ior";

# Make sure the files are gone, so we can wait on them.
unlink $messenger_ior;
unlink $implrepo_ior;
unlink $activator_ior;

$IR = new PerlACE::Process ($implrepo_server, "-d 1 -orbobjrefstyle url -t 5 -o $implrepo_ior");
print ">>> " . $IR->CommandLine() . "\n";
$IR->Spawn();
if (PerlACE::waitforfile_timed ($implrepo_ior, 10) == -1) {
    print STDERR "ERROR: cannot find file <$implrepo_ior>\n";
    $IR->Kill(); 
    exit 1;
}

$imr_init_ref = "-ORBInitRef ImplRepoService=file://$implrepo_ior";

$ACT = new PerlACE::Process ($imr_activator, "-d 1 -orbobjrefstyle url -o $activator_ior $imr_init_ref");
print ">>> " . $ACT->CommandLine() . "\n";
$ACT->Spawn();
if (PerlACE::waitforfile_timed ($activator_ior, 15) == -1) {
    print STDERR "ERROR: cannot find file <$activator_ior>\n";
    $IR->Kill(); 
    $ACT->Kill();
    exit 1;
}


$add_imr = new PerlACE::Process ("$tao_imr",
                                 "$imr_init_ref add MessengerService -c \"$server -orbobjrefstyle url -ORBUseIMR 1 $imr_init_ref\"");
print ">>> " . $add_imr->CommandLine() . "\n";
# We want the tao_imr executable to be found exactly in the path
# given, without being modified by the value of -ExeSubDir.
# So, we tell its Process object to ignore the setting of -ExeSubDir.

$add_imr->IgnoreExeSubDir (1);

## Note : Instead of using tao_imr to generate the ior, it's easy enough
## to just create one by hand. The ior is just a normal corbaloc ior with
## the poa_name of the server and ip address of the imr. 
## (ie corbaloc::localhost:8888/Messengerservice)
## Of course, to do this, you'd have to start the imr on port 8888.
## We use the "tao_imr ior" command, because we don't know which port was used.
$add_imr->SpawnWaitKill(10);
$set_ior = new PerlACE::Process ("$tao_imr",
                                 "$imr_init_ref ior MessengerService -f $messenger_ior");
print ">>> " . $set_ior->CommandLine() . "\n";
$set_ior->IgnoreExeSubDir (1);
$set_ior->SpawnWaitKill(10);
if (PerlACE::waitforfile_timed ($messenger_ior, 5) == -1) {
    print STDERR "ERROR: cannot find file <$messenger_ior>\n";
    $IR->Kill(); 
    $ACT->Kill();
    exit 1;
}

$C1 = new PerlACE::Process("MessengerClient");
print ">>> " . $C1->CommandLine() . "\n";
if ($C1->SpawnWaitKill(10) != 0) {
   print "client 1 timed failed\n";
   $IR->Kill ();
   $ACT->Kill();
   exit 1;
} 

$shutdown = new PerlACE::Process ("$tao_imr",
                                  "$imr_init_ref shutdown MessengerService");
$shutdown->IgnoreExeSubDir (1);
$shutdown->SpawnWaitKill(5);

$C2 = new PerlACE::Process("MessengerClient");
if ($C2->SpawnWaitKill(5) != 0) {
	$IR->Kill ();
        $ACT->Kill();
	exit 1;
} 

$shutdown->SpawnWaitKill(5);

$IR->Kill();
$ACT->Kill();

unlink $messenger_ior;
unlink $implrepo_ior;
unlink $activator_ior;

exit 0;
