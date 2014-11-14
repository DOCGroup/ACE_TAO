# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$mode = "-a NORMAL ";
$lockout = "";
$debugging = 0;
$start_limit = 1;
$number_tries = 2;
$number_succeed = 2;
$imr_debug = "-d 0 ";
$act_debug = "-d 0 ";

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debugging = 1;
        $imr_debug = "-ORBDebugLevel 10 -ORBVerboseLogging 1 -ORBLogFile imr.log -d 5 ";
        $act_debug = "-ORBDebugLevel 10 -ORBVerboseLogging 1 -ORBLogFile imr.log -d 5 ";
    }
    elsif ($i eq '-manual') {
        # in manual mode the server is manually started, so it should
        # just succeed once
        $mode = "-a MANUAL ";
        $number_succeed = 1;
    }
    elsif ($i eq '-lockout') {
        $lockout = " --lockout";
    }
}

my $extra_timeout = 45;

my $seconds_between_requests = 4;

# Valgrind may slow down processes enough that an extra delay
# is needed between client requests.
if (exists $ENV{'ACE_RUN_VALGRIND_CMD'}) {
    $seconds_between_requests = 10;
}

my $c1 = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $imr = PerlACE::TestTarget::create_target (2) || die "Create target 3 failed\n";
my $act = PerlACE::TestTarget::create_target (3) || die "Create target 4 failed\n";
my $ti = PerlACE::TestTarget::create_target (4) || die "Create target 5 failed\n";

my $implrepo_server = "$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/tao_imr_locator";
my $imr_activator = "$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/tao_imr_activator";
my $tao_imr = "$ENV{ACE_ROOT}/bin/tao_imr";

my $implrepo_ior = "implrepo.ior";
my $activator_ior = "activator.ior";
my $messenger_ior = "Messenger.ior";
my $imrlogfile = "imr.log";
my $actlogfile = "act.log";

# Use client strategy factory for one of retry parameters.
$c1_conf = "client.conf";

my $imr_imriorfile = $imr->LocalFile ($implrepo_ior);
my $act_imriorfile = $act->LocalFile ($implrepo_ior);
my $ti_imriorfile = $ti->LocalFile ($implrepo_ior);
my $act_actiorfile = $act->LocalFile ($activator_ior);
my $c1_srviorfile = $c1->LocalFile ($messenger_ior);
my $c1_conffile = $c1->LocalFile ($c1_conf);
my $act_srviorfile = $ti->LocalFile ($messenger_ior);

# Make sure the files are gone, so we can wait on them.
sub delete_files
{
    my $logs_too = shift;
    if ($logs_too == 1) {
        $imr->DeleteFile ($imrlogfile);
        $act->DeleteFile ($actlogfile);
    }
    $imr->DeleteFile ($implrepo_ior);
    $act->DeleteFile ($implrepo_ior);
    $ti->DeleteFile ($implrepo_ior);
    $act->DeleteFile ($activator_ior);
    $act->DeleteFile ($messenger_ior);
    $c1->DeleteFile ($messenger_ior);
}

# Clean up after exit call
END
{
    delete_files (0);
}

delete_files (1);

# Note : We don't actually use SVR, but we need a way to get the
#        path to the -ExeSubDir
$SVR = $imr->CreateProcess ("MessengerServer", "");
my $server = $SVR->Executable ();
my $srv_server = $imr->LocalFile ($server);

$IR = $imr->CreateProcess ($implrepo_server, $imr_debug .
                           "-orbobjrefstyle url ".
                           "-v 1000 -t 5 $lockout ".
                           "-o $imr_imriorfile ");
print ">>> " . $IR->CommandLine() . "\n";

my $initrefbase = "-ORBInitRef ImplRepoService=file://";
my $actinitref = $initrefbase . $act_imriorfile;
my $tiinitref = $initrefbase . $ti_imriorfile;
$ACT = $act->CreateProcess ($imr_activator, " " .
                            $act_debug .
                            "-orbobjrefstyle url " .
                            "-o $act_actiorfile " .
                            $actinitref);

my $client_cmdline = "-k file://$c1_srviorfile ".
        "-ORBForwardOnReplyClosedLimit 20 -ORBForwardDelay 1000 ".
        "-ORBSvcConf $c1_conffile ".
        "-d $seconds_between_requests -t $number_tries -s $number_succeed";
my $locked_client_cmdline = "-k file://$c1_srviorfile ".
        "-ORBForwardOnReplyClosedLimit 20 -ORBForwardDelay 1000 ".
        "-ORBSvcConf $c1_conffile ".
        "-d $seconds_between_requests -t $number_tries -s 0";

$C1 = $c1->CreateProcess ("MessengerClient", $client_cmdline);

$TI = $ti->CreateProcess ($tao_imr);
$TI->IgnoreExeSubDir (1);

sub kill_imr
{
    my $msg = shift;
    print STDERR "ERROR: $msg\n" if (length ($msg) > 0);
    $ACT->Kill (); $ACT->TimedWait (1);
    $IR->Kill (); $IR->TimedWait (1);
    return 1;
}

sub ti_cmd
{
    my $cmd = shift;
    my $cmdargs = shift;

    my $obj_name = "MessengerService";
    print "invoking ti cmd $cmd $obj_name $cmdargs\n";# if ($debugging);
    $TI->Arguments ("$tiinitref $cmd $obj_name $cmdargs");
    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval() + $extra_timeout);
    if ($TI_status != 0 && $TI_status != 4) {
        return kill_imr ("tao_imr $cmd $obj_name returned $TI_status");
    }
    return 0;
}

sub list
{
    $TI->Arguments ("$tiinitref list -v");
    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval() + $extra_timeout);
    if ($TI_status != 0 && $TI_status != 4) {
        return kill_imr ("tao_imr list returned $TI_status");
    }
    return 0;

}

$IR_status = $IR->Spawn ();

if ($IR_status != 0) {
    print STDERR "ERROR: ImplRepo Service returned $IR_status\n";
    exit 1;
}

if ($imr->WaitForFileTimed ($implrepo_ior,$imr->ProcessStartWaitInterval() + $extra_timeout) == -1) {
    print STDERR "ERROR: cannot find file <$imr_imriorfile>\n";
    $IR->Kill (); $IR->TimedWait (1);
    exit 1;
}

if ($imr->GetFile ($implrepo_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$imr_imriorfile>\n";
    $IR->Kill (); $IR->TimedWait (1);
    exit 1;
}
if ($act->PutFile ($implrepo_ior) == -1) {
    print STDERR "ERROR: cannot set file <$act_imriorfile>\n";
    $IR->Kill (); $IR->TimedWait (1);
    exit 1;
}
if ($ti->PutFile ($implrepo_ior) == -1) {
    print STDERR "ERROR: cannot set file <$ti_imriorfile>\n";
    $IR->Kill (); $IR->TimedWait (1);
    exit 1;
}

print ">>> " . $ACT->CommandLine() . "\n";
$ACT_status = $ACT->Spawn ();

if ($ACT_status != 0) {
    print STDERR "ERROR: ImR_Activator returned $ACT_status\n";
    $IR->Kill (); $IR->TimedWait (1);
    exit 1;
}

if ($act->WaitForFileTimed ($activator_ior,$act->ProcessStartWaitInterval() + $extra_timeout) == -1) {
    kill_imr ("cannot find file <$act_actiorfile>");
    exit 1;
}

my $srv_cmdline = "$srv_server -orbobjrefstyle url -ORBUseIMR 1 $actinitref";

if (ti_cmd ("add", $mode . "-c \"$srv_cmdline\" ")) {
    exit 1;
}

if ($mode eq "-a MANUAL " && ti_cmd ("start")) {
    exit 1;
}

if (ti_cmd ("ior", "-f $act_srviorfile")) {
    exit 1;
}
else {
    if ($act->WaitForFileTimed ($messenger_ior,$act->ProcessStartWaitInterval() + $extra_timeout) == -1) {
        kill_imr ("cannot find file <$act_srviorfile>");
        exit 1;
    }

    if ($act->GetFile ($messenger_ior) == -1) {
        kill_imr ("cannot retrieve file <$act_srviorfile>");
        exit 1;
    }

    if ($c1->PutFile ($messenger_ior) == -1) {
        kill_imr ("cannot set file <$c1_srviorfile>");
        exit 1;
    }
}

list ();

print ">>> " . $C1->CommandLine() . "\n";
$C1_status = $C1->SpawnWaitKill ($c1->ProcessStartWaitInterval() + $extra_timeout);

if ($C1_status == 2) {
    print STDERR "Warning: This test does not currently run under this operating system.\n";
    kill_imr ();
    exit 0;
}
elsif ($C1_status != 0) {
    kill_imr ("Client1 returned $C1_status");
    exit 1;
}

if ($lockout eq " --lockout" ) {
    $C1_status = $C1->SpawnWaitKill ($c1->ProcessStartWaitInterval() + $extra_timeout);
    list ();
    if ($mode eq "-a MANUAL ") {
        ti_cmd ("start");
    }
    $C1->Arguments ($locked_client_cmdline);
    $C1_status = $C1->SpawnWaitKill ($c1->ProcessStartWaitInterval() + $extra_timeout);
}

ti_cmd ("shutdown");
kill_imr ();

exit $status;
