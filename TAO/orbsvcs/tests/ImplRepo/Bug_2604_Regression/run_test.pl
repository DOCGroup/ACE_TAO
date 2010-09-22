eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$

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

my $DEBUG_LEVEL = 2;
my $OBJ_REF_STYLE = "-ORBObjRefStyle url";

my $implrepo_server =
    "$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/tao_imr_locator";
my $imr_activator = "$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/ImR_Activator";
my $tao_imr = "$ENV{ACE_ROOT}/bin/tao_imr";

my $imriorfile = "imr.ior";
my $actiorfile = "activator.ior";
my $srviorfile = "messenger.ior";

my $imr_imriorfile = $imr->LocalFile ($imriorfile);
my $act_imriorfile = $act->LocalFile ($imriorfile);
my $ti_imriorfile = $ti->LocalFile ($imriorfile);
my $srv_imriorfile = $srv->LocalFile ($imriorfile);
my $act_actiorfile = $act->LocalFile ($actiorfile);
my $srv_srviorfile = $srv->LocalFile ($srviorfile);
my $cli_srviorfile = $cli->LocalFile ($srviorfile);


$IMR = $imr->CreateProcess ($implrepo_server, "-d $DEBUG_LEVEL ".
                                               "$OBJ_REF_STYLE ".
                                               "-t 30 ".
                                               "-v 100 ".
                                               "-o $imr_imriorfile");
$ACT = $act->CreateProcess ($imr_activator, "-d $DEBUG_LEVEL ".
                                            "$OBJ_REF_STYLE ".
                                            "-o $act_actiorfile ".
                                            "-ORBInitRef ImplRepoService=file://$act_imriorfile");
$TI = $ti->CreateProcess ("$tao_imr");
$SRV = $srv->CreateProcess ("MessengerServer", "-orbuseimr 1 ".
                                               "$OBJ_REF_STYLE ".
                                               "-o $srv_srviorfile ".
                                               "-ORBInitRef ImplRepoService=file://$srv_imriorfile");
$CLI = $cli->CreateProcess ("MessengerClient");

my $server_cmd = $SRV->Executable();
my $srv_server_cmd = $imr->LocalFile ($server_cmd);

sub CleanupOutput {
    $imr->DeleteFile ($imriorfile);
    $act->DeleteFile ($imriorfile);
    $ti->DeleteFile ($imriorfile);
    $srv->DeleteFile ($imriorfile);
    $act->DeleteFile ($actiorfile);
    $srv->DeleteFile ($srviorfile);
    $cli->DeleteFile ($srviorfile);
}

sub SpawnWait {
    my $process = shift;
    my $file = shift;
    my $target = shift;
    my $timeout = shift;

    if (!defined $timeout) {
        $timeout = $target->ProcessStartWaitInterval();
    }

    print ">>> " . $process->CommandLine ()  . "\n\ttimeout = $timeout\n";
    my $status = $process->Spawn ();
    if ($status != 0) {
        print STDERR "ERROR: Process returned $status\n";
        return 1;
    }
    my $ret = $target->WaitForFileTimed ($file,$timeout);
    if ($ret == -1) {
         print STDERR "ERROR: Cannot find file <$file>\n";
    }
    return $ret;
}

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

if (SpawnWait($IMR, $imriorfile, $imr) != 0) {
    $ImR->Kill (); $ImR->TimedWait (1);
    exit 1;
}
if ($imr->GetFile ($imriorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$imr_imriorfile>\n";
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}
if ($ti->PutFile ($imriorfile) == -1) {
    print STDERR "ERROR: cannot set file <$ti_imriorfile>\n";
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}
if ($act->PutFile ($imriorfile) == -1) {
    print STDERR "ERROR: cannot set file <$act_imriorfile>\n";
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}
if ($srv->PutFile ($imriorfile) == -1) {
    print STDERR "ERROR: cannot set file <$srv_imriorfile>\n";
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}

if (SpawnWait($ACT, $actiorfile, $act) != 0) {
    $ACT->Kill (); $ACT->TimedWait (1);
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}

my $actname = $act->HostName ();
RunImRUtil("add MessengerService -a per_client -r 1 -l $actname -c \""
           . "$srv_server_cmd $OBJ_REF_STYLE "
           . "-ORBEndpoint iiop://:8923/portspan=3\"");
RunImRUtil("list -v");

#start the server manually twice

if (SpawnWait($SRV, $srviorfile, $srv, 30) != 0) {
    $SRV->Kill (); $SRV->TimedWait (1);
    $ACT->Kill (); $ACT->TimedWait (1);
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}

$SRV->Kill (); $SRV->TimedWait (1);
$srv->DeleteFile ($srviorfile);

if (SpawnWait($SRV, $srviorfile, $srv, 30) != 0) {
    $SRV->Kill (); $SRV->TimedWait (1);
    $ACT->Kill (); $ACT->TimedWait (1);
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}
$SRV->Kill (); $SRV->TimedWait (1);
if ($srv->GetFile ($srviorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$srv_srviorfile>\n";
    exit 1;
}
if ($cli->PutFile ($srviorfile) == -1) {
    print STDERR "ERROR: cannot set file <$cli_srviorfile>\n";
    exit 1;
}

$CLI_status = $CLI->SpawnWaitKill ($cli->ProcessStartWaitInterval()+985);
if ($CLI_status != 0) {
    print STDERR "Error : Client failed to run correctly with error $CLI_status.";
    $status = 1;
}

$ACT_status = $ACT->TerminateWaitKill ($act->ProcessStopWaitInterval());
if ($ACT_status != 0) {
    print STDERR "ERROR: Activator returned $ACT_status\n";
    $status = 1;
}

$IMR_status = $IMR->TerminateWaitKill ($imr->ProcessStopWaitInterval());
if ($IMR_status != 0) {
    print STDERR "ERROR: ImplRepo Service returned $IMR_status\n";
    $status = 1;
}

CleanupOutput();

exit $status;
