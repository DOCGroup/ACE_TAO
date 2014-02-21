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

# Override default ping interval of 10 seconds so the test
# can run faster when showing the list of active servers
# since server is considered active is pinged within
# ping interval and ping occurs on startup.
my $ping_interval_milliseconds = 1000;

my $servers_count = 2;
my $servers_kill_count = 1;
my $server_init_delay = 0;
my $server_reply_delay = 0;
my $use_activator = 1;

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
	elsif ($ARGV[$i] eq "-servers_to_kill") {
	    $i++;
	    $servers_kill_count = $ARGV[$i];
	}
	elsif ($ARGV[$i] eq "-server_init_delay") {
	    $i++;
	    $server_init_delay = $ARGV[$i];
	}
	elsif ($ARGV[$i] eq "-server_reply_delay") {
	    $i++;
	    $server_reply_delay = $ARGV[$i];
	}
	elsif ($ARGV[$i] eq "-no_activator") {
	    $use_activator = 0;
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
my @srv;
for(my $i = 0; $i < $servers_count; $i++) {
    push (@srv, PerlACE::TestTarget::create_target (++$tgt_num)) || die "Create target $tgt_num failed\n";
}

my $refstyle = " -ORBobjrefstyle URL";
my $obj_count = 1;
my $port = 9876;

my $objprefix = "TestObject";
my $client_wait_time = 10;

$imriorfile = "imr_locator.ior";
$actiorfile = "imr_activator.ior";
$persistxml = "persist.xml";
$persistdat = "persist.dat";

my $imr_imriorfile = $imr->LocalFile ($imriorfile);
my $act_imriorfile = $act->LocalFile ($imriorfile);
my $ti_imriorfile = $ti->LocalFile ($imriorfile);
my $srv_imriorfile = $srv[0]->LocalFile ($imriorfile);
my $act_actiorfile = $act->LocalFile ($actiorfile);
my $imr_persistxml = $imr->LocalFile ($persistxml);
my $imr_persistdat = $imr->LocalFile ($persistdat);

$IMR = $imr->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/tao_imr_locator");
$ACT = $act->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/tao_imr_activator");
$TI = $ti->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_imr");

$CLI = $cli->CreateProcess ("client");
@SRV;
my @srv_server_cmd;
for(my $i = 0; $i < $servers_count; $i++) {
    push (@SRV, $srv[$i]->CreateProcess ("server"));
    my $server_cmd = $SRV[$i]->Executable();
    push (@srv_server_cmd, $imr->LocalFile ($server_cmd));
}
# Make sure the files are gone, so we can wait on them.
$imr->DeleteFile ($imriorfile);
$act->DeleteFile ($imriorfile);
$ti->DeleteFile ($imriorfile);
$srv[0]->DeleteFile ($imriorfile);
$act->DeleteFile ($actiorfile);
$imr->DeleteFile ($persistxml);
$imr->DeleteFile ($persistdat);

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
    $srv[0]->DeleteFile ($imriorfile);
    $act->DeleteFile ($actiorfile);
    $imr->DeleteFile ($persistxml);
    $imr->DeleteFile ($persistdat);

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

sub servers_setup ()
{
    if ($use_activator) {

        $ACT->Arguments ("-l -d 0 -o $act_actiorfile -ORBInitRef ImplRepoService=file://$act_imriorfile");

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
        for(my $i = 0; $i < $servers_count; $i++) {
	    my $status_file_name = $objprefix . "_$i.status";
	    $srv[$i]->DeleteFile ($status_file_name);

	    $TI->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile ".
			    "add $objprefix" . '_' . $i . " -c \"".
			    $srv_server_cmd[i].
			    " -ORBUseIMR 1d -n $i -d $server_init_delay ".
			    "-ORBInitRef ImplRepoService=file://$imr_imriorfile\"");

	    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
	    if ($TI_status != 0) {
		print STDERR "ERROR: tao_imr returned $TI_status\n";
		$ACT->Kill (); $ACT->TimedWait (1);
		$IMR->Kill (); $IMR->TimedWait (1);
		return 1;
	    }
        }

    }

    for(my $i = 0; $i < $servers_count; $i++ ) {

	##### Start server if not using activator #####
        if (! $use_activator) {
	    my $status_file_name = $objprefix . "_$i.status";
	    $srv[$i]->DeleteFile ($status_file_name);
            $SRV[$i]->Arguments ("-ORBUseIMR 1 -n $i -d $server_init_delay -ORBInitRef ".
                             "ImplRepoService=file://$srv_imriorfile");
            $SRV_status = $SRV[$i]->Spawn ();
            if ($SRV_status != 0) {
                print STDERR "ERROR: server returned $SRV_status\n";
                $SRV[$i]->Kill (); $SRV[$i]->TimedWait (1);
                $ACT->Kill (); $ACT->TimedWait (1);
                $IMR->Kill (); $IMR->TimedWait (1);
                return 1;
            }
        } else {
            # For some reason the servers take forever to spawn when using the activator
            $client_wait_time *= $obj_count;
        }

        if ($status == 1) {
            last;
        }
    }

}

sub make_server_requests()
{
    print "Making requests to servers\n";
    # Why is this sleep needed?
    sleep(2);

    ##### Run client against servers to active them #####
    for(my $i = 0; $i < $servers_count; $i++ ) {
	my $status_file_name = $objprefix . "_$i.status";
	if (!use_activator) {
	    # Make sure server has started by looking for its status file
	    if ($srv[$i]->WaitForFileTimed ($status_file_name, $srv[$i]->ProcessStartWaitInterval()) == -1) {
		print STDERR "ERROR: cannot find file <$status_file_name>\n";
		$SRV[$i]->Kill (); $SRV[$i]->TimedWait (1);
		$ACT->Kill (); $ACT->TimedWait (1);
		$IMR->Kill (); $IMR->TimedWait (1);
		return 1;
	    }

	}
	$CLI->Arguments ("-ORBInitRef Test=corbaloc::localhost:$port/$objprefix" . '_' . $i .
			 " -d $server_reply_delay");
	$CLI_status = $CLI->SpawnWaitKill ($cli->ProcessStartWaitInterval());
	if ($CLI_status != 0) {
	    print STDERR "ERROR: client returned $CLI_status\n";
	    $status = 1;
	    last;
	}
    }

    sleep (server_request_delay);
}

sub shutdown_servers($)
{
    my $start_index = shift;
    for(my $i = $start_index; $i < $servers_count; $i++ ) {
	my $status_file_name = $objprefix . "_$i.status";
        # Shutting down any server object within the server will shutdown the whole server
        $TI->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile ".
                        "shutdown $objprefix" . '_' . $i);
        $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
        if ($TI_status != 0) {
            print STDERR "ERROR: tao_imr shutdown returned $TI_status\n";
            $status = 1;
            last;
        }
        if (! $use_activator) {
            $SRV_status = $SRV[$i]->WaitKill($srv[$i]->ProcessStopWaitInterval());
            if ($SRV_status != 0) {
                print STDERR "ERROR: server not shutdown correctly. It returned $SRV_status\n";
                $status = 1;
                last;
            }
        }
	$srv[$i]->DeleteFile ($status_file_name);
    }
}

sub list_active_servers($)
{
    my $list_options = shift;
    my $start_time = time();
    $TI->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile list $list_options");
    # Redirect output so we can count number of lines in output
    redirect_output();
    $result = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
    my $list_time = time() - $start_time;
    restore_output();
    if ($TI_status != 0) {
	print STDERR "ERROR: tao_imr returned $TI_status\n";
	$ACT->Kill (); $ACT->TimedWait (1);
	$IMR->Kill (); $IMR->TimedWait (1);
	return 1;
    }
    open (FILE, $stderr_file) or die "Can't open $stderr_file: $!";
    $active_servers = 0;
    while (<FILE>) {
	print STDERR $_;
	$active_servers++;
    }
    close FILE;
    print STDERR "List took $list_time seconds.\n";
    return $active_servers;
}

sub servers_list_test
{
    print "Running scale test with $servers_count servers and $obj_count objects.\n";

    my $result = 0;
    my $start_time = time();
    $IMR->Arguments ("-d $debug_level -ORBDebugLevel $debug_level -v 1000 -o $imr_imriorfile -orbendpoint iiop://:$port");

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
    if ($srv[0]->PutFile ($imriorfile) == -1) {
        print STDERR "ERROR: cannot set file <$srv_imriorfile>\n";
        $IMR->Kill (); $IMR->TimedWait (1);
        return 1;
    }

    servers_setup();

    # Make sure servers are active whether activator is used or not by making
    # CORBA requests.
    make_server_requests();

    print "\nList of active servers before killing server(s)\n";
    $active_servers_before_kill = list_active_servers("-a");

    # Kill servers and verify listing of active servers is correct.
    print "\nKilling $servers_kill_count servers\n";
    for (my $i = 0; $i < $servers_kill_count; $i++) {
	$CLI->Arguments ("-ORBInitRef Test=corbaloc::localhost:$port/$objprefix" . '_' . $i .
			 " -d $server_reply_delay -a");
	$CLI_status = $CLI->SpawnWaitKill ($cli->ProcessStartWaitInterval());
	if ($CLI_status != 0) {
	    print STDERR "ERROR: client returned $CLI_status\n";
	    $status = 1;
#	    return 1;
	}
	$SRV[$i]->Kill ();
    }

    sleep (2);

    print "\nList of active servers after killing a server\n";
    $active_servers_after_kill = list_active_servers("-a");
    if ($active_servers_after_kill != $active_servers_before_kill - $servers_kill_count) {
	print STDERR
	    "ERROR: Excepted list of active servers after killing ".
	    "a server to be " . ($active_servers_before_kill -  $servers_kill_count) .
	    " but was $active_servers_after_kill\n";
	$status = 1;
    }

    print "\nList of servers registered with the ImR after killing a server\n";
    list_active_servers("");

    print "\n";

    shutdown_servers($servers_kill_count);

    if ($use_activator) {
	my $ACT_status = $ACT->TerminateWaitKill ($act->ProcessStopWaitInterval());
	if ($ACT_status != 0) {
	    print STDERR "ERROR: IMR Activator returned $ACT_status\n";
	    $status = 1;
	}
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
	"[-servers <num=$servers_count>] ".
	"[-servers_to_kill <num=$servers_kill_count>] ".
	"[-server_init_delay <seconds=$server_init_delay>] ".
	"[-server_reply_delay <seconds=$server_reply_delay ".
	"[-no_activator]\n";
}

###############################################################################
###############################################################################

my $ret = servers_list_test();

exit $ret;
