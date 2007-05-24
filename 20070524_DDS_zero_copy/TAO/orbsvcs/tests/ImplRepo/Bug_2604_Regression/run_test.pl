eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$

use Env (ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::Run_Test;
use Sys::Hostname;

my $WAIT_TIMEOUT = $PerlACE::wait_interval_for_process_creation;
my $DEBUG_LEVEL = 2;
my $OBJ_REF_STYLE = "-ORBObjRefStyle url";

my $implrepo_server =
    "$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/ImplRepo_Service";
my $imr_activator = "$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/ImR_Activator";
my $tao_imr = "$ENV{ACE_ROOT}/bin/tao_imr";

my $implrepo_ior = PerlACE::LocalFile("imr.ior");
my $activator_ior = PerlACE::LocalFile("activator.ior");
my $messenger_ior = PerlACE::LocalFile("messenger.ior");
my $messenger_bak = PerlACE::LocalFile("messenger.bak");

my $imr_init_ref = "-ORBInitRef ImplRepoService=file://$implrepo_ior";

my $Svr = new PerlACE::Process("MessengerServer",
                               "-orbuseimr 1 $OBJ_REF_STYLE $imr_init_ref");
my $Cli = new PerlACE::Process("MessengerClient");

my $server_cmd = $Svr->Executable();

sub CleanupOutput {
    unlink $messenger_ior;
    unlink $implrepo_ior;
    unlink $activator_ior;
    unlink $messenger_bak;
}

sub SpawnWait {
    my $process = shift;
    my $file = shift;
    my $timeout = shift;
    if (!defined $timeout) {
        $timeout = $WAIT_TIMEOUT;
    }

    print ">>> " . $process->CommandLine() . "\n\ttimeout = $timeout\n";
    $process->Spawn();
    my $ret = PerlACE::waitforfile_timed($file, $timeout);
    if ($ret == -1) {
         print STDERR "ERROR: Cannot find file <$file>\n";
    }
    return $ret;
}

my $ImR = new PerlACE::Process ($implrepo_server, "-d $DEBUG_LEVEL "
                                ."$OBJ_REF_STYLE -t 30 -v 1000 "
                                ."-o $implrepo_ior ");
my $Act = new PerlACE::Process ($imr_activator, "-l -d $DEBUG_LEVEL "
                                ."$OBJ_REF_STYLE -o $activator_ior "
                                ."$imr_init_ref");

my $imr_util = new PerlACE::Process ("$tao_imr");
# We want the tao_imr executable to be found exactly in the path
# given, without being modified by the value of -ExeSubDir.
# So, we tell its Process object to ignore the setting of -ExeSubDir.
$imr_util->IgnoreExeSubDir(1);

sub RunImRUtil {
    my $cmd = shift;
    print ">>> " . $imr_util->CommandLine() . "\n";
    $imr_util->Arguments("$imr_init_ref $cmd");
    return $imr_util->SpawnWaitKill(5);
}

CleanupOutput();

if (SpawnWait($ImR, $implrepo_ior) != 0) {
    $ImR->Kill();
    exit 1;
}

if (SpawnWait($Act, $activator_ior) != 0) {
    $ImR->Kill();
    exit 1;
}

my $actname = hostname;
RunImRUtil("add MessengerService -a per_client -r 1 -l $actname -c \""
           . "$server_cmd $OBJ_REF_STYLE "
           . "-ORBEndpoint iiop://:8923/portspan=3\"");
RunImRUtil("list -v");

#start the server manually twice

if (SpawnWait($Svr, $messenger_ior, 30) != 0) {
    $Act->Kill();
    $ImR->Kill();
    exit 1;
}
$Svr->Kill();
unlink $messenger_ior;

if (SpawnWait($Svr, $messenger_ior, 30) != 0) {
    $Act->Kill();
    $ImR->Kill();
    exit 1;
}
$Svr->Kill();

if ($Cli->SpawnWaitKill(1000) != 0) {
    print STDERR "Error : Client failed to run correctly.";
    $Act->Kill();
    $ImR->Kill();
    CleanupOutput();
    exit 1;
}

$Act->Kill();
$ImR->Kill();

CleanupOutput();

exit 0;
