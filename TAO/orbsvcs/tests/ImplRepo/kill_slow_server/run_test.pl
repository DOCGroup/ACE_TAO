eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

###############################################################################
use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = 0;
$no_dns = 0;
$imrhost = "127.0.0.1";
$poa_delay = 20;

if ($#ARGV >= 0) {
    for (my $i = 0; $i <= $#ARGV; $i++) {
	if ($ARGV[$i] eq '-debug') {
	    $debug_level = 10;
	}
        elsif ($ARGV[$i] eq '-no_dns') {
            $no_dns = 1;
        }
        elsif ($ARGV[$i] eq '-p') {
            $i++;
            $poa_delay = $ARGV[$i];
        }
	else {
	    usage();
	    exit 1;
	}
    }
}

$tgt_num = 0;
$imr = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
$act = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
$ti  = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
$tinw = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
$cli = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
$clinw = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
$srv = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";

$refstyle = " -ORBobjrefstyle URL";
$imrport = 9876 + $imr->RandomPort ();
$imrhost = $imr->HostName () if ($no_dns == 0);

$statusfile = "TestObject.status";
$client_wait_time = 30;

$imriorfile = "imr_locator.ior";
$actiorfile = "imr_activator.ior";

$actlogfile = "act.log";
$cltlogfile = "client.log";
$imrlogfile = "imr.log";
$srvlogfile = "server.log";

$imr_imriorfile = $imr->LocalFile ($imriorfile);
$act_imriorfile = $act->LocalFile ($imriorfile);
$ti_imriorfile = $ti->LocalFile ($imriorfile);
$srv_imriorfile = $srv->LocalFile ($imriorfile);
$act_actiorfile = $act->LocalFile ($actiorfile);


$IMR = $imr->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/tao_imr_locator");
$ACT = $act->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/tao_imr_activator");
$TI = $ti->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_imr");
$TINW = $tinw->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_imr");

$CLI = $cli->CreateProcess ("client");
$CLINW = $clinw->CreateProcess ("client");
$SRV = $srv->CreateProcess ("server");
$server_cmd = $SRV->Executable();
$srv_server_cmd = $imr->LocalFile ($server_cmd);

$stdout_file      = "test.out";
$stderr_file      = "test.err";
$ti_stdout_file = $ti->LocalFile ($stdout_file);
$ti_stderr_file = $ti->LocalFile ($stderr_file);

sub deletefiles
{
    my $logs_too = shift;

    $imr->DeleteFile ($imriorfile);
    $act->DeleteFile ($imriorfile);
    $ti->DeleteFile ($imriorfile);
    $srv->DeleteFile ($imriorfile);
    $act->DeleteFile ($actiorfile);

    $ti->DeleteFile ($stdout_file);
    $ti->DeleteFile ($stderr_file);

    $srv->DeleteFile ($statusfile);

    if ($logs_too == 1) {
        $imr->DeleteFile ($imrlogfile);
        $act->DeleteFile ($actlogfile);
        $cli->DeleteFile ($cltlogfile);
        $srv->DeleteFile ($srvlogfile);
    }
}

# Clean up after exit call
END
{
    deletefiles (0);
}

sub redirect_output
{
    open(OLDOUT, ">&", \*STDOUT) or die "Can't dup STDOUT: $!";
    open(OLDERR, ">&", \*STDERR) or die "Can't dup STDERR: $!";
    open STDERR, '>', $ti_stderr_file;
    open STDOUT, '>', $ti_stdout_file;
}

sub restore_output
{
    open(STDERR, ">&OLDERR") or die "Can't dup OLDERR: $!";
    open(STDOUT, ">&OLDOUT") or die "Can't dup OLDOUT: $!";
}

sub register_server
{
    my $expected = shift;
    my $debugarg = "-ORBVerboseLogging 1 -ORBDebugLevel $debug_level -ORBLogfile $srvlogfile" if ($debug_level > 0);
    my $endpointarg = "-ORBDottedDecimalAddresses 1 -ORBListenEndpoints iiop://127.0.0.1:" if ($no_dns == 1);

    $TI->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile ".
                    "add TestObject_a -c \"".
                    $srv_server_cmd .
                    " -ORBUseIMR 1 -p $poa_delay " .
                    "$debugarg $endpointarg " .
                    "-ORBInitRef ImplRepoService=file://$imr_imriorfile\"");

    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
    if ($TI_status != $expected) {
        print STDERR "ERROR: tao_imr returned $TI_status\n";
        $ACT->Kill (); $ACT->TimedWait (1);
        $IMR->Kill (); $IMR->TimedWait (1);
        $status = 1;
    }
}

sub act_setup
{
    my $debugarg = "-d 2 -ORBVerboseLogging 1 -ORBDebugLevel $debug_level -ORBLogfile $actlogfile " if ($debug_level > 0);
    my $endpointarg = "-ORBDottedDecimalAddresses 1 -ORBListenEndpoints iiop://127.0.0.1: " if ($no_dns == 1);

    $ACT->Arguments ("$debugarg $endpointarg -l -o $act_actiorfile ".
                     "-ORBInitRef ImplRepoService=file://$act_imriorfile  ");

    $ACT_status = $ACT->Spawn ();
    if ($ACT_status != 0) {
        print STDERR "ERROR: ImR Activator returned $ACT_status\n";
        return 1;
    }
    if ($act->WaitForFileTimed ($actiorfile, $act->ProcessStartWaitInterval())== -1) {
        print STDERR "ERROR: cannot find file <$act_imriorfile>\n";
        $ACT->Kill (); $ACT->TimedWait (1);
        $IMR->Kill (); $IMR->TimedWait (1);
        return 1;
    }
}

sub run_client
{
    my $killit = shift;
    my $debugarg = "-ORBVerboseLogging 1 -ORBDebugLevel $debug_level -ORBLogfile $cltlogfile " if ($debug_level > 0);
    my $endpointarg = "-orbdotteddecimaladdresses 1" if ($no_dns == 1);

    $CLI->Arguments ("-ORBInitRef Test=corbaloc::$imrhost:$imrport/TestObject_a ".
                     "$debugarg $endpointarg $killit");

    $CLI_status = $CLI->SpawnWaitKill ($client_wait_time);
    if ($CLI_status != 0) {
        print STDERR "ERROR: client returned $CLI_status\n";
        $status = 1;
    }
}

sub start_client_no_wait
{
    my $debugarg = "-ORBVerboseLogging 1 -ORBDebugLevel $debug_level -ORBLogfile $cltlogfile " if ($debug_level > 0);
    my $endpointarg = "-orbdotteddecimaladdresses 1" if ($no_dns == 1);

    $CLINW->Arguments ("-ORBInitRef Test=corbaloc::$imrhost:$imrport/TestObject_a ".
                       "$debugarg $endpointarg");

    $CLINW_status = $CLINW->Spawn ();
    if ($CLINW_status != 0) {
        print STDERR "ERROR: client nw returned $CLINW_status\n";
        $status = 1;
    }
}

sub shutdown_server
{
    # Shutting down any server object within the server will shutdown the whole server
    $TI->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile ".
                    "shutdown TestObject_a" );
    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
    if ($TI_status != 0) {
        print STDERR "ERROR: tao_imr shutdown returned $TI_status\n";
        $status = 1;
    }
}

sub kill_server
{
    # Shutting down any server object within the server will shutdown the whole server
    $TI->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile ".
                    "kill TestObject_a -s 15" );
    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
    if ($TI_status != 0) {
        print STDERR "ERROR: tao_imr shutdown returned $TI_status\n";
        $status = 1;
    }
}

sub start_server_no_wait
{
    # Shutting down any server object within the server will shutdown the whole server
    $TINW->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile ".
                      "start TestObject_a" );
    $TI_status = $TINW->Spawn ($tinw->ProcessStartWaitInterval());

    if ($TI_status != 0) {
        print STDERR "ERROR: tao_imr start returned $TI_status\n";
        $status = 1;
    }
}

sub start_server_complete
{
    $TI_status = $TINW->WaitKill ($tinw->ProcessStartWaitInterval());

    if ($TI_status != 0) {
        print STDERR "ERROR: tao_imr start returned $TI_status\n";
        $status = 1;
    }
}

sub manual_start_server
{
    # Shutting down any server object within the server will shutdown the whole server
    $TI->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile ".
                    "start TestObject_a" );
    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
    if ($TI_status != 0) {
        print STDERR "ERROR: tao_imr start returned $TI_status\n";
        $status = 1;
    }
}

sub validate_servers
{
    open FILE, "<$statusfile";
    @pids = <FILE>;
    $count = scalar @pids;
    print "$statusfile has $count ($#pids) lines\n";

    if ($count != 2) {
        print STDERR "ERROR: expected 2 server pids, got $count\n";
        for (my $i = 0; $i <= $#pids; $i++) {
            print STDERR "killing server pid[$i] $pids[$i]\n";
            kill $pids[$i];
        }
    }
}


sub double_server_test
{
    print "Running slow server kill test\n";
    my $debugarg = "-d 10 -ORBVerboseLogging 1 -ORBDebugLevel $debug_level -ORBLogfile $imrlogfile " if ($debug_level > 0);
    my $endpointarg = "-orbdotteddecimaladdresses 1" if ($no_dns == 1);

    my $result = 0;
    my $start_time = time();
    $IMR->Arguments ("$debugarg -v 800 -o $imr_imriorfile $endpointarg -ORBListenEndpoints iiop://$imrhost:$imrport");

    ##### Start ImplRepo #####
    $IMR_status = $IMR->Spawn ();
    if ($IMR_status != 0) {
        print STDERR "ERROR: ImplRepo Service returned $IMR_status\n";
        return 1;
    }
    if ($imr->WaitForFileTimed ($imriorfile, $imr->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$imr_imriorfile>\n";
        $IMR->Kill (); $IMR->TimedWait (1);
        return 1;
    }
    if ($imr->GetFile ($imriorfile) == -1) {
        print STDERR "ERROR: cannot retrieve file <$imr_imriorfile>\n";
        $IMR->Kill (); $IMR->TimedWait (1);
        return 1;
    }
    if ($act->PutFile ($imriorfile) == -1) {
        print STDERR "ERROR: cannot set file <$act_imriorfile>\n";
        $IMR->Kill (); $IMR->TimedWait (1);
        return 1;
    }
    if ($ti->PutFile ($imriorfile) == -1) {
        print STDERR "ERROR: cannot set file <$ti_imriorfile>\n";
        $IMR->Kill (); $IMR->TimedWait (1);
        return 1;
    }
    if ($srv->PutFile ($imriorfile) == -1) {
        print STDERR "ERROR: cannot set file <$srv_imriorfile>\n";
        $IMR->Kill (); $IMR->TimedWait (1);
        return 1;
    }

    print "Start activator\n";
    act_setup();

    print "Register server\n";
    register_server(0);

    if ($status != 0) {
        return 1;
    }

    print "Manual start\n";
    manual_start_server ();

    if ($status != 0) {
        return 1;
    }

    print "Shutdown server\n";
    shutdown_server ();

    sleep (1);

    print "killing server\n";
    kill_server ();

    print "second manual start\n";
    manual_start_server ();

    print "killing server\n";
    kill_server ();

    my $CLINW_status = $CLINW->TerminateWaitKill ($clinw->ProcessStopWaitInterval());
    if ($CLINW_status != 0) {
	print STDERR "ERROR: no-wait client returned $CLINW_status\n";
	$status = 1;
    }

    my $ACT_status = $ACT->TerminateWaitKill ($act->ProcessStopWaitInterval());
    if ($ACT_status != 0) {
        print STDERR "ERROR: IMR Activator returned $ACT_status\n";
        $status = 1;
    }

    my $IMR_status = $IMR->TerminateWaitKill ($imr->ProcessStopWaitInterval());
    if ($IMR_status != 0) {
	print STDERR "ERROR: IMR returned $IMR_status\n";
	$status = 1;
    }

    $status = validate_servers();

    my $test_time = time() - $start_time;

    print "\nFinished. The test took $test_time seconds.\n";

    return $status;
}

sub usage() {
    print "Usage: run_test.pl ".
	"[-debug]\n";
}

###############################################################################
###############################################################################

deletefiles (1);

my $ret = double_server_test();

exit $ret;
