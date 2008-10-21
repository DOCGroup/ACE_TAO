# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use Env (ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::Run_Test;

my $WAIT_TIMEOUT = 2;
my $DEBUG_LEVEL = 1;
my $OBJ_REF_STYLE = "-orbobjrefstyle url";

my $implrepo_server = "$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/ImplRepo_Service";
my $imr_activator = "$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/ImR_Activator";
my $tao_imr = "$ENV{ACE_ROOT}/bin/tao_imr";

my $implrepo_ior = "imr.ior";
my $activator_ior = "activator.ior";
my $messenger1_ior = "messenger1.ior";
my $messenger2_ior = "messenger2.ior";
my $messenger3_ior = "messenger3.ior";
my $messenger4_ior = "messenger4.ior";

my $imr_init_ref = "-ORBInitRef ImplRepoService=file://$implrepo_ior";

my $Svr = new PerlACE::Process('MessengerServer', "-orbuseimr 1 $OBJ_REF_STYLE $imr_init_ref");
my $Cli = new PerlACE::Process('MessengerClient');

sub CleanupOutput {
    unlink $messenger1_ior;
    unlink $messenger2_ior;
    unlink $messenger3_ior;
    unlink $messenger4_ior;
    unlink $implrepo_ior;
    unlink $activator_ior;
}

sub SpawnWait {
    my $process = shift;
    my $file = shift;

    print ">>> " . $process->CommandLine() . "\n";
    $process->Spawn();
    my $ret = PerlACE::waitforfile_timed($file, $WAIT_TIMEOUT);
    if ($ret == -1) {
         print STDERR "ERROR: Cannot find file <$file>\n";
    }
    return $ret;
}

sub SpawnWaitKill {
    my $process = shift;

    print ">>> " . $process->CommandLine() . "\n";
    return $process->SpawnWaitKill($WAIT_TIMEOUT);
}

# Use url object reference style for readability, and startup timeout of 2 seconds.
# Unlike the chapter we'll forgo using -m, because we want to be able to run this
# as a test in our nightly builds, and multicast could interfere with other machines.
my $ImR = new PerlACE::Process ($implrepo_server, "-d $DEBUG_LEVEL $OBJ_REF_STYLE -t 2 -o $implrepo_ior");
my $Act = new PerlACE::Process ($imr_activator, "-d $DEBUG_LEVEL $OBJ_REF_STYLE -o $activator_ior $imr_init_ref");

my $imr_util = new PerlACE::Process ("$tao_imr",
    "$imr_init_ref add MessengerService -c \"$Svr->Executable() $OBJ_REF_STYLE -ORBUseIMR 1 $imr_init_ref\"");

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

#### Start the example

if (SpawnWait($ImR, $implrepo_ior) != 0) {
    $ImR->Kill(); 
    exit 1;
}

if (SpawnWait($Svr, $messenger2_ior) != 0) {
    $ImR->Kill();
    exit 1;
}

$Cli->Arguments($messenger1_ior);
if (SpawnWaitKill($Cli, $WAIT_TIMEOUT) != 0) {
    print STDERR "Error : Client 1 failed to run correctly.";
}

$Cli->Arguments($messenger2_ior);
if (SpawnWaitKill($Cli, $WAIT_TIMEOUT) != 0) {
    print STDERR "Error : Client 2 failed to run correctly.";
}

#Since we didn't start the ImR with a known endpoint, we'll
#have to use the tao_imr utility to create a our urls.
RunImRUtil("ior MessengerService/Object1 -f $messenger3_ior");
RunImRUtil("ior MessengerService/Object2 -f $messenger4_ior");

$Cli->Arguments($messenger3_ior);
if (SpawnWaitKill($Cli, $WAIT_TIMEOUT) != 0) {
    print STDERR "Error : Client 3 failed to run correctly.";
}

# Notice that this one results in two calls through the ImR, because
# we bound an indirect reference in the IORTable. If we were using
# PER_CLIENT activation, this would be bad.
$Cli->Arguments($messenger4_ior);
if (SpawnWaitKill($Cli, $WAIT_TIMEOUT) != 0) {
    print STDERR "Error : Client 4 failed to run correctly.";
}

$Svr->Kill();
$ImR->Kill();


#### Clean up any output files

CleanupOutput();

exit 0;
