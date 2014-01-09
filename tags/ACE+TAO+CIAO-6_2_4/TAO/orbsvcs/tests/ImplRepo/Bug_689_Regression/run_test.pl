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
my $srv = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $cli = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";

my $imriorfile = "imr.ior";
my $actiorfile = "act.ior";
my $srviorfile = "server.ior";

my $imr_imriorfile = $imr->LocalFile ($imriorfile);
my $act_imriorfile = $act->LocalFile ($imriorfile);
my $ti_imriorfile = $ti->LocalFile ($imriorfile);
my $srv_imriorfile = $srv->LocalFile ($imriorfile);
my $act_actiorfile = $act->LocalFile ($actiorfile);
my $srv_srviorfile = $srv->LocalFile ($srviorfile);
my $cli_srviorfile = $cli->LocalFile ($srviorfile);
$imr->DeleteFile ($imriorfile);
$act->DeleteFile ($imriorfile);
$ti->DeleteFile ($imriorfile);
$srv->DeleteFile ($imriorfile);
$act->DeleteFile ($actiorfile);
$srv->DeleteFile ($srviorfile);
$cli->DeleteFile ($srviorfile);

$IMR = $imr->CreateProcess ("../../../ImplRepo_Service/tao_imr_locator",
                            "-o $imr_imriorfile");
$ACT = $act->CreateProcess ("../../../ImplRepo_Service/tao_imr_activator",
                            "-o $act_actiorfile ".
                            "-ORBInitRef ImplRepoService=file://$act_imriorfile");
$TI = $ti->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_imr");
$CLI = $cli->CreateProcess ("client",
                            "-k file://$cli_srviorfile -ORBDebugLevel $debug_level");
$SRV = $srv->CreateProcess ("server");

my $server_cmd = $SRV->Executable();
my $srv_server_cmd = $imr->LocalFile ($server_cmd);

$TI->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile ".
                "add test_server -c \"$srv_server_cmd ".
                "-ORBInitRef ImplRepoService=file://$imr_imriorfile ".
                "-ORBUseIMR 1 -ORBDebugLevel $debug_level ".
                "-o $srv_srviorfile\"");


# We want the tao_imr executable to be found exactly in the path
# given, without being modified by the value of -ExeSubDir.
# So, we tell its Process object to ignore the setting of -ExeSubDir.
$TI->IgnoreExeSubDir(1);

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
if ($srv->PutFile ($imriorfile) == -1) {
    print STDERR "ERROR: cannot set file <$srv_imriorfile>\n";
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}

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

$TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
if ($TI_status != 0) {
    print STDERR "ERROR: TAO IMR returned $TI_status\n";
    $ACT->Kill (); $ACT->TimedWait (1);
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}

$TI->Arguments("-ORBInitRef ImplRepoService=file://$ti_imriorfile" . ' start test_server');

$TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
if ($TI_status != 0) {
    print STDERR "ERROR: TAO IMR returned $TI_status\n";
    $ACT->Kill (); $ACT->TimedWait (1);
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}

if ($srv->GetFile ($srviorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$srv_srviorfile>\n";
    $ACT->Kill (); $ACT->TimedWait (1);
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}
if ($cli->PutFile ($srviorfile) == -1) {
    print STDERR "ERROR: cannot set file <$cli_srviorfile>\n";
    $ACT->Kill (); $ACT->TimedWait (1);
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}


$CLI_status = $CLI->SpawnWaitKill ($cli->ProcessStartWaitInterval());
if ($CLI_status != 0) {
    print STDERR "ERROR: client returned $CLI_status\n";
    $status = 1;
}

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
$srv->DeleteFile ($imriorfile);
$act->DeleteFile ($actiorfile);
$srv->DeleteFile ($srviorfile);
$cli->DeleteFile ($srviorfile);

exit $status;
