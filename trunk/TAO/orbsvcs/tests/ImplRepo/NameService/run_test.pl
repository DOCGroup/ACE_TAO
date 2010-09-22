eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

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

$ACT = $act->CreateProcess ("../../../ImplRepo_Service/ImR_Activator");

$NS = $ns->CreateProcess ("../../../Naming_Service/tao_cosnaming");
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

$IMR->Arguments ("-o $imr_imriorfile -d 2");
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

$ACT->Arguments("-d 1 -o $act_actiorfile -ORBInitRef ImplRepoService=file://$act_imriorfile");
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
## Register the NameService

$TI->Arguments("-ORBInitRef ImplRepoService=file://$ti_imriorfile"
               . " add NameService -c \""
               . "$ns_ns_cmd -ORBInitRef ImplRepoService=file://$imr_imriorfile -ORBUseIMR 1"
               . "\"");

$TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval()+45);
if ($TI_status != 0) {
    print STDERR "ERROR: TAO IMR returned $TI_status\n";
    $ACT->Kill (); $ACT->TimedWait (1);
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}

################################################################################
## Create IOR for NameService

$TI->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile ior NameService -f $ns_nsiorfile");
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

$TEST->Arguments ("-ORBInitRef NameService=file://$test_nsiorfile");
$TEST_status = $TEST->SpawnWaitKill ($test->ProcessStartWaitInterval()+45);
if ($TEST_status != 0) {
    print STDERR "ERROR: test returned $TEST_status\n";
    $status = 1;
}

################################################################################
## Shutdown the NameService

$TI->Arguments ("-ORBInitRef ImplRepoService=file://$imr_imriorfile shutdown "
                     . "NameService ");
$TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval()+45);
if ($TI_status != 0) {
    print STDERR "ERROR: TAO IMR returned $TI_status\n";
    $status = 1;
}

################################################################################
## Kill the IMR

$ACT_status = $ACT->TerminateWaitKill ($act->ProcessStopWaitInterval());

if ($ACT_status != 0) {
    print STDERR "ERROR: IMR Activator returned $ACT_status\n";
    $status = 1;
}

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
