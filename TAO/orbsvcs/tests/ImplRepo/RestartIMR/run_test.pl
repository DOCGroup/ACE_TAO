eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

###############################################################################

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;

my $tgt_num = 0;
my $imr = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
my $act = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
my $ti  = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
my $srv = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";

my $port = $imr->RandomPort ();
my $srv_id = "AAA";
my $srv_id_cmd = "-ORBServerId AAA ";
my $ping_ext = "-v 0 ";
my $server_pid = 0;

my $kill_svr = 0;
my $kill_act = 0;

my $imr_debug = "-d 1 ";
my $act_debug = "";
my $srv_debug = "";
my $clt_debug = "";

foreach my $i (@ARGV) {
    if ($i eq '-debug') {
        $imr_debug = "-ORBDebugLevel 10 -ORBVerboseLogging 1 -ORBLogFile imr.log -d 5 ";
        $act_debug = "-ORBDebugLevel 10 -ORBVerboseLogging 1 -ORBLogFile act.log -d 5 ";
        $srv_debug = "-ORBDebugLevel 10 -ORBVerboseLogging 1 -ORBLogFile srv.log ";
    }
    elsif ($i eq '-kill_act') {
        $kill_act = 1;
        $kill_svr = 1;
    }
    elsif ($i eq '-kill_server') {
        $kill_svr = 1;
    }
    elsif ($i eq '-noid') {
        $srv_id = "";
        $srv_id_cmd = "";
    }
    elsif ($i eq '-pingexternal') {
        $ping_ext = "-i ";
    }
    else {
        print STDERR "unrecognized argument $i\n";
        exit 1;
    }
}

my $imriorfile = "imr_locator.ior";
my $imrlogfile = "imr.log";
my $actiorfile = "imr_activator.ior";
my $actlogfile = "act.log";
my $srvlogfile = "srv.log";
my $srvpidfile = "server.pid";
my $persist_dir = "persist";

my $imr_imriorfile = $imr->LocalFile ($imriorfile);
my $act_imriorfile = $act->LocalFile ($imriorfile);
my $act_actiorfile = $act->LocalFile ($actiorfile);
my $srv_imriorfile = $srv->LocalFile ($imriorfile);
my $srv_srvpidfile = $srv->LocalFile ($srvpidfile);
my $ti_imriorfile = $ti->LocalFile ($imriorfile);


sub get_server_pid
{
    my $pid = 0;
    open (FILE, $srv_srvpidfile) or die "Can't open server.pid: $!";
    while (<FILE>) {
        chomp;
        $pid = $_;
        $server_pid = $pid if ($server_pid == 0);
    }
    close FILE;
    return $pid;
}

sub signal_server
{
    my $sig = shift;
    print "signal $sig to server $server_pid\n";
    kill ($sig, $server_pid);
}


sub delete_files
{
    my $logs_too = shift;
    if ($logs_too == 1) {
        $imr->DeleteFile ($imrlogfile);
        $act->DeleteFile ($actlogfile);
        $srv->DeleteFile ($srvlogfile);
    }
    $imr->DeleteFile ($imriorfile);
    $act->DeleteFile ($imriorfile);
    $act->DeleteFile ($actiorfile);
    $ti->DeleteFile ($imriorfile);
    $srv->DeleteFile ($imriorfile);
    $srv->DeleteFile ($srvpidfile);

    unlink <persist/*>;
    rmdir $persist_dir;
}

# Clean up after exit call
END
{
    delete_files (0);
}

delete_files (1);

mkdir $persist_dir;

$IMR = $imr->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/tao_imr_locator");
$ACT = $act->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/tao_imr_activator");
$TI = $ti->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_imr");

$initref = "-ORBInitRef ImplRepoService=file://$ti_imriorfile";

$IMR->Arguments ("-ORBEndpoint iiop://:$port ".
                 "-UnregisterIfAddressReused $imr_debug".
                 "--directory $persist_dir " .
                 $ping_ext .
                 "-o $imr_imriorfile ");

$ACT->Arguments ("-l $act_debug -o $act_actiorfile " .
                        "-ORBInitRef ImplRepoService=file://$act_imriorfile");

my $srv_args = "-ORBInitRef ImplRepoService=file://$srv_imriorfile -ORBUseIMR 1 ".
    $srv_id_cmd . $srv_debug;

$SRV = $srv->CreateProcess ("server", $srv_args);

my $poaA = "$srv_id:poaA";
my $poaC = "$srv_id:poaC";

my $srv_cmd = $act->LocalFile ($SRV->Executable());

$TI->Arguments ("$initref add $poaA -c \"$srv_cmd $srv_args\"");

print STDERR "=== start Locator\n";
print STDERR $IMR->CommandLine () . "\n";
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
if ($srv->PutFile ($imriorfile) == -1) {
    print STDERR "ERROR: cannot set file <$srv_imriorfile>\n";
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

print STDERR "=== start Activator\n";
$ACT_status = $ACT->Spawn ();
if ($ACT_status != 0) {
    print STDERR "ERROR: ImR Activator returned $ACT_status\n";
    exit 1;
}
if ($act->WaitForFileTimed ($actiorfile,$act->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$act_imriorfile>\n";
    $ACT->Kill (); $ACT->TimedWait (1);
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}

print STDERR "=== Registering POAs\n";
$TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
if ($TI_status != 0) {
    print STDERR "tao_imr register poaA returned $TI_status\n";
}
$TI->Arguments ("$initref link $poaA -p poaC");
$TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
if ($TI_status != 0) {
    print STDERR "tao_imr link poaC to poaA returned $TI_status\n";
}

print STDERR "=== start server\n";
$TI->Arguments ("$initref start $poaA");
$TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
if ($TI_status != 0) {
    print STDERR "tao_imr list returned $TI_status\n";
}
if ($srv->GetFile ($srvpidfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$srvpidfile>\n";
}

print STDERR "=== list pre-kill\n";
$TI->Arguments ("$initref list -v");
$TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
if ($TI_status != 0) {
    print STDERR "tao_imr list poaC to poaA returned $TI_status\n";
}

print STDERR "=== kill ImR Locator\n";
$IMR_status = $IMR->TerminateWaitKill ($imr->ProcessStopWaitInterval());
$imr->DeleteFile ($imriorfile);

if ($kill_act == 1) {
    print STDERR "=== kill ImR Activator\n";
    $ACT_status = $ACT->TerminateWaitKill ($act->ProcessStopWaitInterval());
    $act->DeleteFile ($actiorfile);
}

if ($kill_svr == 1) {
    print STDERR "=== kill server\n";
    get_server_pid ();
    signal_server ("KILL");
    $srv->DeleteFile ($srvpidfile);
}

print STDERR "=== restart ImR Locator\n";
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

if ($kill_act == 1) {
    print STDERR "=== restart ImR Activator\n";
    $ACT_status = $ACT->Spawn ();
    if ($ACT_status != 0) {
        print STDERR "ERROR: ImplRepo Service returned $ACT_status\n";
        $IMR->Kill (); $IMR->TimedWait (1);
        $ACT->Kill (); $ACT->TimedWait (1);
        exit 1;
    }
    if ($act->WaitForFileTimed ($actiorfile,$act->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$act_actiorfile>\n";
        $IMR->Kill (); $IMR->TimedWait (1);
        $ACT->Kill (); $ACT->TimedWait (1);
        exit 1;
    }
}

print STDERR "=== restart server via ImR\n";
$TI->Arguments ("$initref start $poaA ");
$TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
if ($TI_status != 0) {
    print STDERR "tao_imr start $poaA returned $TI_status\n";
}
$TI->Arguments ("$initref list -v");
$TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
if ($TI_status != 0) {
    print STDERR "tao_imr list returned $TI_status\n";
}

$TI->Arguments ("$initref shutdown $poaA ");
$TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
if ($TI_status != 0) {
    print STDERR "tao_imr shutdown $poaA returned $TI_status\n";
}

$ACT_status = $ACT->TerminateWaitKill ($act->ProcessStopWaitInterval());

$IMR_status = $IMR->TerminateWaitKill ($imr->ProcessStopWaitInterval());

exit $status;
