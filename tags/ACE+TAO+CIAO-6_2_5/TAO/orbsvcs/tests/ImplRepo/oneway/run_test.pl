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

if ($#ARGV >= 0) {
    for (my $i = 0; $i <= $#ARGV; $i++) {
	if ($ARGV[$i] eq '-debug') {
	    $debug_level = '10';
	    $i++;
	}
	elsif ($ARGV[$i] eq "-servers") {
	    $i++;
	    $servers_count = $ARGV[$i];
	}
	else {
	    usage();
	    exit 1;
	}
    }
}

#$ENV{ACE_TEST_VERBOSE} = "1";

my $tgt_num = 0;
my $imr = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
my $act = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
my $ti  = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
my $cli = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
my $cli_ds = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
my $srv = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";

my $refstyle = " -ORBobjrefstyle URL";
my $port = 9876;

my $status_file_name = "TestObject.status";
my $client_wait_time = 10;

$imriorfile = "imr_locator.ior";
$actiorfile = "imr_activator.ior";

my $imr_imriorfile = $imr->LocalFile ($imriorfile);
my $act_imriorfile = $act->LocalFile ($imriorfile);
my $ti_imriorfile = $ti->LocalFile ($imriorfile);
my $srv_imriorfile = $srv->LocalFile ($imriorfile);
my $act_actiorfile = $act->LocalFile ($actiorfile);

$IMR = $imr->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/tao_imr_locator");
$ACT = $act->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/tao_imr_activator");
$TI = $ti->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_imr");

$CLI = $cli->CreateProcess ("client");
$CLI_DS = $cli_ds->CreateProcess ("client_ds");
$SRV = $srv->CreateProcess ("server");
my $srv_server_cmd =  $imr->LocalFile ($SRV->Executable());

# Make sure the files are gone, so we can wait on them.
$imr->DeleteFile ($imriorfile);
$act->DeleteFile ($imriorfile);
$ti->DeleteFile ($imriorfile);
$srv->DeleteFile ($imriorfile);
$act->DeleteFile ($actiorfile);

my $stdout_file      = "test.out";
my $stderr_file      = "test.err";
my $ti_stdout_file = $ti->LocalFile ($stdout_file);
my $ti_stderr_file = $ti->LocalFile ($stderr_file);

# Clean up after exit call
END
{
    $imr->DeleteFile ($imriorfile);
    $act->DeleteFile ($imriorfile);
    $ti->DeleteFile ($imriorfile);
    $srv->DeleteFile ($imriorfile);
    $act->DeleteFile ($actiorfile);

    $ti->DeleteFile ($stdout_file);
    $ti->DeleteFile ($stderr_file);

    # Remove any stray server status files caused by aborting services
    unlink <*.status>;
}

sub redirect_output()
{
    open(OLDOUT, ">&", \*STDOUT) or die "Can't dup STDOUT: $!";
    open(OLDERR, ">&", \*STDERR) or die "Can't dup STDERR: $!";
    open STDERR, '>', $ti_stderr_file;
    open STDOUT, '>', $ti_stdout_file;
}

sub restore_output()
{
    open(STDERR, ">&OLDERR") or die "Can't dup OLDERR: $!";
    open(STDOUT, ">&OLDOUT") or die "Can't dup OLDOUT: $!";
}

sub server_setup ()
{
    $ACT->Arguments ("-d 2 -o $act_actiorfile -ORBInitRef ImplRepoService=file://$act_imriorfile");

    $ACT_status = $ACT->Spawn ();
    if ($ACT_status != 0) {
        print STDERR "ERROR: ImR Activator returned $ACT_status\n";
        return 1;
    }
    if ($act->WaitForFileTimed ($actiorfile,$act->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$act_imriorfile>\n";
        $ACT->Kill (); $ACT->TimedWait (1);
        $IMR->Kill (); $IMR->TimedWait (1);
        return 1;
    }

    ##### Add servers to activator #####
    $srv->DeleteFile ($status_file_name);
    $TI->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile ".
                    "add TestObject_a -c \"".
                    $srv_server_cmd.
                    " -o server.ior -ORBUseIMR 1 ".
                    "-ORBInitRef ImplRepoService=file://$imr_imriorfile\"");

    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
    if ($TI_status != 0) {
        print STDERR "ERROR: tao_imr returned $TI_status\n";
        $ACT->Kill (); $ACT->TimedWait (1);
        $IMR->Kill (); $IMR->TimedWait (1);
        return 1;
    }
}

sub make_server_requests($$)
{
    my $usefile = shift;
    my $proc = shift;

    my $init_ref = "-ORBInitRef Test=";
    if ($usefile == 0) {
        $init_ref .= "corbaloc::1.2\@localhost:$port/TestObject_a";
    }
    else {
        $init_ref .= "file://server.ior";
    }
    my $svc_conf = "-ORBSvcConf defsync.conf" if ($proc == $CLI_DS);
    my $debug_args = "-ORBDebugLevel $debug_level -ORBLogFile client.log" if ($debug_level > 0);

    print "Making requests using $init_ref $svc_conf $debug_args\n";
    $proc->Arguments ("$init_ref $svc_conf $debug_args" );

    $CLI_status = $proc->SpawnWaitKill ($cli->ProcessStartWaitInterval());
    if ($CLI_status != 0) {
        print STDERR "ERROR: client returned $CLI_status\n";
        $status = 1;
    }
}

sub shutdown_server()
{
    # Shutting down any server object within the server will shutdown the whole server
    $TI->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile ".
                    "shutdown TestObject_a");
    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
    if ($TI_status != 0) {
        print STDERR "ERROR: tao_imr kill returned $TI_status\n";
        $status = 1;
    }
    $srv->DeleteFile ($status_file_name);
}

sub oneway_sync_test
{
    print "Running oneway sync test.\n";

    my $start_time = time();
    my $debug_args = "-d $debug_level -ORBDebugLevel $debug_level -ORBLogFile imr.log" if ($debug_level > 0);
    $IMR->Arguments ("-v 1000 -o $imr_imriorfile -orbendpoint iiop://:$port $debug_args");

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

    server_setup ();

    make_server_requests(0, $CLI);
    make_server_requests(1, $CLI);
    make_server_requests(1, $CLI_DS);

    shutdown_server ();

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

    my $test_time = time() - $start_time;

    print "\nFinished. The test took $test_time seconds.\n";

    return $status;
}

sub usage() {
    print "Usage: run_test.pl ".
	"[-servers <num=$servers_count>]\n";
}

###############################################################################
###############################################################################

my $ret = oneway_sync_test();

exit $ret;
