eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '2';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $imr = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $act = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $ti = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $ns = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $test = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";

################################################################################
# Program locations

$imriorfile = "imr.ior";
$nsiorfile = "name.ior";
$actiorfile = "activator.ior";

my $imr_imriorfile = $imr->LocalFile ($imriorfile);
my $act_imriorfile = $act->LocalFile ($imriorfile);
my $ti_imriorfile = $ti->LocalFile ($imriorfile);
my $ns_imriorfile = $ns->LocalFile ($imriorfile);
my $act_actiorfile = $act->LocalFile ($actiorfile);
my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $test_nsiorfile = $test->LocalFile ($nsiorfile);

$IMR = $imr->CreateProcess ("../../../ImplRepo_Service/tao_imr_locator");

$ACT = $act->CreateProcess ("../../../ImplRepo_Service/tao_imr_activator");

$NS = $ns->CreateProcess ("../../../Notify_Service/tao_cosnotification");
my $ns_cmd = $NS->Executable();
my $ns_ns_cmd = $imr->LocalFile ($ns_cmd);

$TI = $ti->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_imr");
# We want the tao_imr executable to be found exactly in the path
# given, without being modified by the value of -ExeSubDir.
# So, we tell its Process object to ignore the setting of -ExeSubDir.
$TI->IgnoreExeSubDir (1);

$TEST = $test->CreateProcess ("test");

################################################################################

$imr->DeleteFile ($imriorfile);
$act->DeleteFile ($imriorfile);
$ti->DeleteFile ($imriorfile);
$ns->DeleteFile ($imriorfile);
$act->DeleteFile ($actiorfile);
$ns->DeleteFile ($nsiorfile);
$test->DeleteFile ($nsiorfile);

################################################################################
## Start the implementation Repository

print "-------> Starting IMR Locator\n";
$IMR->Arguments ("-o $imr_imriorfile -d $debug_level");
$IMR_status = $IMR->Spawn ();
if ($IMR_status != 0) {
    print STDERR "ERROR: ImplRepo Service returned $IMR_status\n";
    exit 1;
}
if ($imr->WaitForFileTimed ($imriorfile,$imr->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$imr_imriorfile>\n";
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}
if ($imr->GetFile ($imriorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$imr_imriorfile>\n";
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}
if ($act->PutFile ($imriorfile) == -1) {
    print STDERR "ERROR: cannot set file <$act_imriorfile>\n";
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}

if ($ti->PutFile ($imriorfile) == -1) {
    print STDERR "ERROR: cannot set file <$ti_imriorfile>\n";
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}
if ($ns->PutFile ($imriorfile) == -1) {
    print STDERR "ERROR: cannot set file <$ns_imriorfile>\n";
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}

print "-------> Starting IMR Activator\n";
$ACT->Arguments("-d 1 -l -o $act_actiorfile -ORBInitRef ImplRepoService=file://$act_imriorfile  -ORBDebugLevel $debug_level");
$ACT_status = $ACT->Spawn ();
if ($ACT_status != 0) {
    print STDERR "ERROR: IMR Activator Service returned $ACT_status\n";
    exit 1;
}
if ($act->WaitForFileTimed ($actiorfile,$act->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$act_actiorfile>\n";
    $ACT->Kill (); $ACT->TimedWait (1);
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}

################################################################################
## Register the NotifyService

print "-------> Register service\n";

$TI->Arguments("-ORBInitRef ImplRepoService=file://$ti_imriorfile"
               . " add CosNotify:1 -c \""
               . "$ns_cmd -boot -NoNameSvc -Factory CosNotify:1/NotifyService -ORBServerId CosNotify -ORBInitRef ImplRepoService=file://$imr_imriorfile -ORBUseIMR 1 -ORBSvcConf ntfy.conf"
               . "\"");

$TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval()+45);
if ($TI_status != 0) {
    print STDERR "ERROR: TAO IMR returned $TI_status\n";
    $ACT->Kill (); $ACT->TimedWait (1);
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}


################################################################################
## Create IOR for NotifyService

print "-------> Create IOR\n";
$TI->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile ior CosNotify:1/NotifyService -f $ns_nsiorfile");
$TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval()+45);
if ($TI_status != 0) {
    print STDERR "ERROR: TAO IMR returned $TI_status\n";
    $ACT->Kill (); $ACT->TimedWait (1);
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}
if ($ns->WaitForFileTimed ($nsiorfile,$ns->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ns_nsiorfile>\n";
    $ACT->Kill (); $ACT->TimedWait (1);
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}
if ($ns->GetFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ns_nsiorfile>\n";
    $ACT->Kill (); $ACT->TimedWait (1);
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}
if ($test->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$test_nsiorfile>\n";
    $ACT->Kill (); $ACT->TimedWait (1);
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}

################################################################################
## Run the test

print "-------> Run test\n";
$TEST->Arguments ("-ORBInitRef NotifyService=file://$test_nsiorfile -ORBDebugLevel $debug_level");
$TEST_status = $TEST->SpawnWaitKill ($test->ProcessStartWaitInterval()+45);
if ($TEST_status != 0) {
    print STDERR "ERROR: test returned $TEST_status\n";
    $status = 1;
}

################################################################################
## Shutdown the NotifyService

print "-------> Shutdown service\n";
$TI->Arguments ("-ORBInitRef ImplRepoService=file://$imr_imriorfile shutdown CosNotify:1 ");
$TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval()+45);
if ($TI_status != 0) {
    print STDERR "ERROR: TAO IMR returned $TI_status\n";
    $status = 1;
}

################################################################################
## Kill the IMR

print "-------> Shutdown ImR Activator\n";
$ACT_status = $ACT->TerminateWaitKill ($act->ProcessStopWaitInterval());

if ($ACT_status != 0) {
    print STDERR "ERROR: IMR Activator returned $ACT_status\n";
    $status = 1;
}

print "-------> Shutdown ImR Locator\n";
$IMR_status = $IMR->TerminateWaitKill ($imr->ProcessStopWaitInterval());

if ($IMR_status != 0) {
    print STDERR "ERROR: ImplRepo Service returned $IMR_status\n";
    $status = 1;
}

$imr->DeleteFile ($imriorfile);
$act->DeleteFile ($imriorfile);
$ti->DeleteFile ($imriorfile);
$ns->DeleteFile ($imriorfile);
$act->DeleteFile ($actiorfile);
$ns->DeleteFile ($nsiorfile);
$test->DeleteFile ($nsiorfile);

exit $status;
