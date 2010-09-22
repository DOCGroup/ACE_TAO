# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $c1 = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $imr = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $act = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $ti = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";

my $WAIT_TIMEOUT = 2;
my $DEBUG_LEVEL = 1;
my $OBJ_REF_STYLE = "-orbobjrefstyle url";

my $implrepo_server = "$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/tao_imr_locator";
my $imr_activator = "$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/ImR_Activator";
my $tao_imr = "$ENV{ACE_ROOT}/bin/tao_imr";

my $implrepo_ior = "imr.ior";
my $activator_ior = "activator.ior";
my $messenger_ior = "messenger.ior";

my $imr_imriorfile = $imr->LocalFile ($implrepo_ior);
my $act_imriorfile = $act->LocalFile ($implrepo_ior);
my $ti_imriorfile = $ti->LocalFile ($implrepo_ior);
my $act_actiorfile = $act->LocalFile ($activator_ior);
my $imr_srviorfile = $imr->LocalFile ($messenger_ior);
my $c1_srviorfile = $c1->LocalFile ($messenger_ior);


sub CleanupOutput {
    $imr->DeleteFile ($implrepo_ior);
    $act->DeleteFile ($implrepo_ior);
    $ti->DeleteFile ($implrepo_ior);
    $act->DeleteFile ($activator_ior);
    $imr->DeleteFile ($messenger_ior);
    $c1->DeleteFile ($messenger_ior);
}

sub SpawnWait {
    my $process = shift;
    my $file = shift;
    my $target = shift;

    print ">>> " . $process->CommandLine () . "\n";
    my $status = $process->Spawn ();
    if ($status != 0) {
        print STDERR "ERROR: Process returned $status\n";
        return 1;
    }
    my $ret = $target->WaitForFileTimed ($file,$target->ProcessStartWaitInterval());
    if ($ret == -1) {
         print STDERR "ERROR: Cannot find file <$file>\n";
    }
    return $ret;
}


$Srv = $imr->CreateProcess ("../Basic/MessengerServer", "-ORBdebuglevel $debug_level ".
                                            "-orbuseimr 1 ".
                                            "$OBJ_REF_STYLE ".
                                            "-ORBInitRef ImplRepoService=file://$imr_imriorfile");
$C1 = $c1->CreateProcess ("../Basic/MessengerClient", "-k file://$c1_srviorfile ");
my $server_cmd = $Srv->Executable();
my $srv_server_cmd = $imr->LocalFile ($server_cmd);

# Use url object reference style for readability, and startup timeout of 2 seconds.
# Unlike the chapter we'll forgo using -m, because we want to be able to run this
# as a test in our nightly builds, and multicast could interfere with other machines.
$ImR = $imr->CreateProcess ($implrepo_server, "-d $DEBUG_LEVEL ".
                                               "$OBJ_REF_STYLE ".
                                               "-t 2 ".
                                               "-o $imr_imriorfile");
$Act = $act->CreateProcess ($imr_activator, "-d $DEBUG_LEVEL ".
                                               "$OBJ_REF_STYLE ".
                                               "-o $act_actiorfile ".
                                               "-ORBInitRef ImplRepoService=file://$act_imriorfile");

$TI = $ti->CreateProcess ($tao_imr);
# We want the tao_imr executable to be found exactly in the path
# given, without being modified by the value of -ExeSubDir.
# So, we tell its Process object to ignore the setting of -ExeSubDir.
$TI->IgnoreExeSubDir(1);

sub RunImRUtil {
    my $cmd = shift;

    print ">>> " . $TI->CommandLine() . "\n";
    $TI->Arguments("-ORBInitRef ImplRepoService=file://$ti_imriorfile $cmd");
    return $TI->SpawnWaitKill($ti->ProcessStartWaitInterval());
}

CleanupOutput();

#### Start the example

if (SpawnWait($ImR, $implrepo_ior, $imr) != 0) {
    $ImR->Kill(); $ImR->TimedWait(1);
    exit 1;
}
if ($imr->GetFile ($implrepo_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$imr_imriorfile>\n";
    $ImR->Kill (); $ImR->TimedWait (1);
    exit 1;
}
if ($act->PutFile ($implrepo_ior) == -1) {
    print STDERR "ERROR: cannot set file <$act_imriorfile>\n";
    $ImR->Kill (); $ImR->TimedWait (1);
    exit 1;
}
if ($ti->PutFile ($implrepo_ior) == -1) {
    print STDERR "ERROR: cannot set file <$ti_imriorfile>\n";
    $ImR->Kill (); $ImR->TimedWait (1);
    exit 1;
}

if (SpawnWait($Act, $activator_ior, $act) != 0) {
    $ImR->Kill(); $ImR->TimedWait(1);
    exit 1;
}

#instead of using tao_imr add, we'll use tao_imr update, because
#we want to run the server once to generate the ior file.

if (SpawnWait($Srv, $messenger_ior, $imr) != 0) {
    $Act->Kill(); $Act->TimedWait(1);
    $ImR->Kill(); $ImR->TimedWait(1);
    exit 1;
}
if ($imr->GetFile ($implrepo_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$imr_srviorfile>\n";
    $Act->Kill(); $Act->TimedWait(1);
    $ImR->Kill (); $ImR->TimedWait (1);
    exit 1;
}
if ($c1->PutFile ($implrepo_ior) == -1) {
    print STDERR "ERROR: cannot set file <$c1_srviorfile>\n";
    $Act->Kill(); $Act->TimedWait(1);
    $ImR->Kill (); $ImR->TimedWait (1);
    exit 1;
}

RunImRUtil("list -v");

RunImRUtil("shutdown MessengerService");
$Srv_status = $Srv->TerminateWaitKill ($imr->ProcessStopWaitInterval());

if ($Srv_status != 0) {
    print STDERR "ERROR: Server returned $Srv_status\n";
    exit 1;
}

RunImRUtil("list -v");

# Note : If the server registers itself, then it won't set the
# activator name. If we don't set it here, then the activator
# won't be able to start the server.
my $actname = $ti->hostname;
RunImRUtil("update MessengerService -l $actname -c \"$srv_server_cmd $OBJ_REF_STYLE -ORBUseIMR 1 -ORBInitRef ImplRepoService=file://$imr_imriorfile\"");

RunImRUtil("list -v");

$C1_status = $C1->SpawnWaitKill ($c1->ProcessStartWaitInterval()+985);

if ($C1_status != 0) {
    print STDERR "ERROR: Client returned $C1_status\n";
    $ImR->Kill (); $ImR->TimedWait (1);
    $ACT->Kill (); $ACT->TimedWait (1);
    exit 1;
}


$Act->Kill();
$Act_status = $Act->TerminateWaitKill ($act->ProcessStopWaitInterval());

if ($Act_status != 0) {
    print STDERR "ERROR: ImR_Activator returned $Act_status\n";
    exit 1;
}

RunImRUtil("list -v");

RunImRUtil("shutdown MessengerService");

$ImR_status = $ImR->TerminateWaitKill ($imr->ProcessStopWaitInterval());

if ($ImR_status != 0) {
    print STDERR "ERROR: ImplRepo Service returned $ImR_status\n";
    $status = 1;
}

#### Clean up any output files

CleanupOutput();

exit $status;
