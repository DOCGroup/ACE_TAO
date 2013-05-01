eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

###############################################################################

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';
my $imr_debug_level = 0;

my $imr = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $srva = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $srvb = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $cli = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";

my $forward_opt = "-ORBForwardOnceOnObjectNotExist 1";
my $delay = 0;
my $got_object_not_exist_exception = 0;
my $protocol = "iiop";
my $port = $imr->RandomPort ();
my $srv_port_base = $srva->RandomPort ();
my $srv_a_id = "AAA";
my $srv_b_id = "BBB";
my $client_duration = 30;
my $ping_ext = '';

foreach my $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
        $imr_debug_level = '1';
    }
    if ($i eq '-forwardalways') {
        $delay = '5';
        $forward_opt = "-ORBForwardInvocationOnObjectNotExist 1";
    }
    if ($i eq '-forwardonce') {
        $delay = '5';
        $forward_opt = "-ORBForwardOnceOnObjectNotExist 1";
        $got_object_not_exist_exception = 1;
    }
    if ($i eq '-pingexternal') {
        $delay = '5';
        $forward_opt = "-ORBForwardOnceOnObjectNotExist 1";
        $ping_ext = '-i';
    }
}

my $imriorfile = "imr_locator.ior";
my $imrdbfile = "imr.db";
my $srvaiorfile = "A.ior";
my $srvbiorfile = "B.ior";

my $imr_imriorfile = $imr->LocalFile ($imriorfile);
my $srva_imriorfile = $srva->LocalFile ($imriorfile);
my $srvb_imriorfile = $srvb->LocalFile ($imriorfile);
my $imr_imrdbfile = $imr->LocalFile ($imrdbfile);
my $srva_srvaiorfile = $srva->LocalFile ($srvaiorfile);
my $srvb_srvbiorfile = $srvb->LocalFile ($srvbiorfile);
my $cli_srvaiorfile = $cli->LocalFile ($srvaiorfile);

$imr->DeleteFile ($imriorfile);
$srva->DeleteFile ($imriorfile);
$srvb->DeleteFile ($imriorfile);
$imr->DeleteFile ($imrdbfile);
$srva->DeleteFile ($srvaiorfile);
$srvb->DeleteFile ($srvbiorfile);
$cli->DeleteFile ($srvaiorfile);


$IMR = $imr->CreateProcess ("../../../ImplRepo_Service/tao_imr_locator",
                            "-ORBEndpoint "."$protocol"."://:".$port." ".
                            "-UnregisterIfAddressReused ".
                            "-d $imr_debug_level ".
                            "$ping_ext ".
                            "-o $imr_imriorfile ");
#                            "-p $imr_imrdbfile");
$SRV_A = $srva->CreateProcess ("serverA",
                               "_ORBDebugLevel = $debug_level ".
                               "-ORBEndpoint " . "$protocol" . "://:" . "$srv_port_base/portspan=20 ".
                               "-ORBInitRef ImplRepoService=file://$srva_imriorfile ".
                               "-ORBServerId $srv_a_id ".
                               "-ORBUseIMR 1 ".
                               "-o $srva_srvaiorfile");
$SRV_B = $srvb->CreateProcess ("serverB",
                               "_ORBDebugLevel = $debug_level ".
                               "-ORBEndpoint " . "$protocol" . "://:" . "$srv_port_base/portspan=20 ".
                               "-ORBInitRef ImplRepoService=file://$srvb_imriorfile ".
                               "-ORBServerId $srv_b_id ".
                               "-ORBUseIMR 1 ".
                               "-o $srvb_srvbiorfile");
$CLI = $cli->CreateProcess ("client",
                            "$forward_opt -i file://$cli_srvaiorfile ".
                            "-t $client_duration ".
                            "-e $got_object_not_exist_exception ");


print STDERR $IMR->CommandLine () . "\n";
$IMR_status = $IMR->Spawn ();
print STDERR "command " .$IMR->CommandLine () . "\n";
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
if ($srva->PutFile ($imriorfile) == -1) {
    print STDERR "ERROR: cannot set file <$srva_imriorfile>\n";
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}
if ($srvb->PutFile ($imriorfile) == -1) {
    print STDERR "ERROR: cannot set file <$srvb_imriorfile>\n";
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}

#sleep (2);
print STDERR "=== start server A: " . $SRV_A->CommandLine () . "\n";
$SRVA_status = $SRV_A->Spawn ();
print STDERR "command " . $SRV_A->CommandLine () . "\n";
if ($SRVA_status != 0) {
    print STDERR "ERROR: Server A returned $SRVA_status\n";
    exit 1;
}
if ($srva->WaitForFileTimed ($srvaiorfile,$srva->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$srva_srvaiorfile>\n";
    $SRV_A->Kill (); $SRV_A->TimedWait (1);
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}
if ($srva->GetFile ($srvaiorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$srva_srvaiorfile>\n";
    $SRV_A->Kill (); $SRV_A->TimedWait (1);
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}
if ($cli->PutFile ($srvaiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$cli_srvaiorfile>\n";
    $SRV_A->Kill (); $SRV_A->TimedWait (1);
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}

print STDERR "=== start server B: " . $SRV_B->CommandLine () . "\n";
$SRVB_status = $SRV_B->Spawn ();
print STDERR "command " .$SRV_B->CommandLine () . "\n";
if ($SRVB_status != 0) {
    print STDERR "ERROR: Server B returned $SRVB_status\n";
    exit 1;
}
if ($srvb->WaitForFileTimed ($srvbiorfile,$srvb->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$srvb_srvbiorfile>\n";
    $SRV_B->Kill (); $SRV_B->TimedWait (1);
    $SRV_A->Kill (); $SRV_A->TimedWait (1);
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}

print STDERR "=== start client: " . $CLI->CommandLine () . "\n";
$CLI_status = $CLI->Spawn ();
if ($CLI_status != 0) {
    print STDERR "ERROR: Client returned $CLI_status\n";
    $CLI->Kill (); $CLI->TimedWait (1);
    $SRV_B->Kill (); $SRV_B->TimedWait (1);
    $SRV_A->Kill (); $SRV_A->TimedWait (1);
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}

sleep (5);

print STDERR "=== kill server A\n";
$SRVA_status = $SRV_A->TerminateWaitKill ($srva->ProcessStopWaitInterval());
if ($SRVA_status != 0) {
    print STDERR "ERROR: Server A returned $SRVA_status\n";
    $CLI->Kill (); $CLI->TimedWait (1);
    $SRV_B->Kill (); $SRV_B->TimedWait (1);
    $SRV_A->Kill (); $SRV_A->TimedWait (1);
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}

print STDERR "=== kill server B\n";
$SRVB_status = $SRV_B->TerminateWaitKill ($srvb->ProcessStopWaitInterval());
if ($SRVB_status != 0) {
    print STDERR "ERROR: Server B returned $SRVB_status\n";
    $CLI->Kill (); $CLI->TimedWait (1);
    $SRV_B->Kill (); $SRV_B->TimedWait (1);
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}

sleep (5);
$srva->DeleteFile ($srvaiorfile);
$srvb->DeleteFile ($srvbiorfile);
$cli->DeleteFile ($srvaiorfile);

print STDERR "=== restart server B\n";
# Run -ORBDebugLevel 10 to see server raise OBJECT_NOT_EXIST exception.
$SRV_B = $srvb->CreateProcess ("serverB",
                               "_ORBDebugLevel = $debug_level ".
                               "-ORBEndpoint " . "$protocol" . "://:" . "$srv_port_base/portspan=20 ".
                               "-ORBInitRef ImplRepoService=file://$srvb_imriorfile ".
                               "-ORBServerId $srv_b_id ".
                               "-ORBUseIMR 1 ".
                               "-o $srvb_srvbiorfile ".
                               "-l $delay");
print STDERR $SRV_B->CommandLine () . "\n";
$SRVB_status = $SRV_B->Spawn ();
if ($SRVB_status != 0) {
    print STDERR "ERROR: Server B returned $SRVB_status\n";
    exit 1;
}
if ($srvb->WaitForFileTimed ($srvbiorfile,$srvb->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$srvb_srvbiorfile>\n";
    $CLI->Kill (); $CLI->TimedWait (1);
    $SRV_B->Kill (); $SRV_B->TimedWait (1);
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}

sleep ($delay * 2);

print STDERR "=== restart server A\n";
print STDERR $SRV_A->CommandLine () . "\n";
$SRVA_status = $SRV_A->Spawn ();
if ($SRVA_status != 0) {
    print STDERR "ERROR: Server A returned $SRVA_status\n";
    exit 1;
}
if ($srva->WaitForFileTimed ($srvaiorfile,$srva->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$srva_srvaiorfile>\n";
    $SRV_A->Kill (); $SRV_A->TimedWait (1);
    $CLI->Kill (); $CLI->TimedWait (1);
    $SRV_B->Kill (); $SRV_B->TimedWait (1);
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}
if ($srva->GetFile ($srvaiorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$srva_srvaiorfile>\n";
    $SRV_A->Kill (); $SRV_A->TimedWait (1);
    $CLI->Kill (); $CLI->TimedWait (1);
    $SRV_B->Kill (); $SRV_B->TimedWait (1);
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}
if ($cli->PutFile ($srvaiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$cli_srvaiorfile>\n";
    $SRV_A->Kill (); $SRV_A->TimedWait (1);
    $CLI->Kill (); $CLI->TimedWait (1);
    $SRV_B->Kill (); $SRV_B->TimedWait (1);
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}

$CLI_status = $CLI->WaitKill ($cli->ProcessStartWaitInterval()+$client_duration);
if ($CLI_status != 0) {
    print STDERR "ERROR: Client returned $CLI_status\n";
    $status = 1;
}

$IMR_status = $IMR->TerminateWaitKill ($imr->ProcessStopWaitInterval());
if ($IMR_status != 0) {
    print STDERR "ERROR: ImplRepo Service returned $IMR_status\n";
    $status = 1;
}
$SRVA_status = $SRV_A->TerminateWaitKill ($srva->ProcessStopWaitInterval());
if ($SRVA_status != 0) {
    print STDERR "Error : Server A returned $SRVA_status.";
    $status = 1;
}
$SRVB_status = $SRV_B->TerminateWaitKill ($srvb->ProcessStopWaitInterval());
if ($SRVB_status != 0) {
    print STDERR "Error : Server B returned $SRVB_status.";
    $status = 1;
}

# Make sure the files are gone, so we can wait on them.
$imr->DeleteFile ($imriorfile);
$srva->DeleteFile ($imriorfile);
$srvb->DeleteFile ($imriorfile);
$imr->DeleteFile ($imrdbfile);
$srva->DeleteFile ($srvaiorfile);
$srvb->DeleteFile ($srvbiorfile);
$cli->DeleteFile ($srvaiorfile);

exit $status;
