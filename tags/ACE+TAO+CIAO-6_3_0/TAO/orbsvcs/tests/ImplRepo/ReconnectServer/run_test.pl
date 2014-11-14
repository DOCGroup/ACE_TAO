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
my $ti =  PerlACE::TestTarget::create_target (5) || die "Create target 4 failed\n";

my $forward_opt = "-ORBForwardOnceOnObjectNotExist 1";
my $delay = 0;
my $got_object_not_exist_exception = 0;
my $protocol = "iiop";
my $port = $imr->RandomPort ();
my $srv_port_base = $srva->RandomPort ();
my $srv_a_id = "-ORBServerId AAA ";
my $srv_b_id = "-ORBServerId BBB ";
my $client_duration = 30;
my $do_link = 0;
my $ping_ext = "-v 0 ";
my $use_java = 0;

my $imr_debug = "";
my $srva_debug = "";
my $srvb_debug = "";
my $clt_debug = "";


foreach my $i (@ARGV) {
    if ($i eq '-debug') {
        $imr_debug = "-ORBDebugLevel 10 -ORBVerboseLogging 1 -ORBLogFile imr.log -d 10 ";
        $srva_debug = "-ORBDebugLevel 10 -ORBVerboseLogging 1 -ORBLogFile srva.log ";
        $srvb_debug = "-ORBDebugLevel 10 -ORBVerboseLogging 1 -ORBLogFile srvb.log ";
        $clt_debug = "-ORBDebugLevel 10 -ORBVerboseLogging 1 -ORBLogFile clt.log ";
    }
    elsif ($i eq '-java') {
        $use_java = 1;
        $delay = 5;
    }
    elsif ($i eq '-link') {
        $do_link = 1;
    }
    elsif ($i eq '-noid') {
        $srv_a_id = "";
        $srv_b_id = "";
    }
    elsif ($i eq '-forwardalways') {
        $delay = 5;
        $forward_opt = "-ORBForwardInvocationOnObjectNotExist 1";
    }
    elsif ($i eq '-forwardonce') {
        $delay = 5;
        $forward_opt = "-ORBForwardOnceOnObjectNotExist 1";
        $got_object_not_exist_exception = 1;
    }
    elsif ($i eq '-pingexternal') {
        $delay = 5;
        $forward_opt = "-ORBForwardOnceOnObjectNotExist 1";
        $ping_ext = "-i ";
    }
    else {
        print STDERR "unrecognized argument $i\n";
        exit 1;
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
my $ti_imriorfile = $ti->LocalFile ($imriorfile);

$imr->DeleteFile ($imriorfile);
$srva->DeleteFile ($imriorfile);
$srvb->DeleteFile ($imriorfile);
$imr->DeleteFile ($imrdbfile);
$srva->DeleteFile ($srvaiorfile);
$srvb->DeleteFile ($srvbiorfile);
$cli->DeleteFile ($srvaiorfile);
$ti->DeleteFile ($imriorfile);

$IMR = $imr->CreateProcess ("../../../ImplRepo_Service/tao_imr_locator",
                            "-ORBEndpoint "."$protocol"."://:".$port." ".
                            "-UnregisterIfAddressReused $imr_debug".
                            $ping_ext .
                            "-o $imr_imriorfile ");

$SRV_A = $srva->CreateProcess ("serverA",
                               "-ORBEndpoint " . $protocol . "://:" ."$srv_port_base/portspan=20 ".
                               "-ORBInitRef ImplRepoService=file://$srva_imriorfile ".
                               $srv_a_id .
                               $srva_debug .
                               "-ORBUseIMR 1 ".
                               "-o $srva_srvaiorfile");
$SRV_B = $srvb->CreateProcess ("serverB",
                               "-ORBEndpoint " . $protocol . "://:" . "$srv_port_base/portspan=20 ".
                               "-ORBInitRef ImplRepoService=file://$srvb_imriorfile ".
                               $srv_b_id .
                               $srvb_debug .
                               "-ORBUseIMR 1 ".
                               "-o $srvb_srvbiorfile");
if ($use_java == 1) {
    $CLI = $cli->CreateProcess ($ENV{'JACORB_HOME'} . "/bin/jaco",
                                "-cp build/classes taoimrtest.reconnectserver.Client");
}
else {
    $CLI = $cli->CreateProcess ("client",
                                "$forward_opt -i file://$cli_srvaiorfile ".
                                "-t $client_duration $clt_debug".
                                "-e $got_object_not_exist_exception ");
}

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

if ($do_link == 1) {
    my $poaA = "AAA:" if ($srv_a_id =~ /AAA/);
    $poaA .= "poaA";
    my $poaC = "AAA:" if ($srv_a_id =~ /AAA/);
    $poaC .= "poaC";

    $TI = $ti->CreateProcess ("../../../ImplRepo_Service/tao_imr",
                              "-ORBInitRef ImplRepoService=file://$srva_imriorfile ".
                              "add $poaA -c serverA");
    if ($ti->PutFile ($imriorfile) == -1) {
        print STDERR "ERROR: cannot set file <$ti_imriorfile>\n";
        $IMR->Kill (); $IMR->TimedWait (1);
        exit 1;
    }
    print STDERR "=== linking POAs for serverA\n";
    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
    if ($TI_status != 0) {
        print STDERR "tao_imr register poaA returned $TI_status\n";
    }
    $TI->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile ".
                    "link $poaA -p poaC");
    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
    if ($TI_status != 0) {
        print STDERR "tao_imr link poaC to poaA returned $TI_status\n";
    }
    $TI->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile ".
                    "list -v");
    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
    if ($TI_status != 0) {
        print STDERR "tao_imr link poaC to poaA returned $TI_status\n";
    }


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
                               $srv_b_id .
                               $srvb_debug .
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
