eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-

###############################################################################
use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use English;

my $status = 0;
my $imr_debug = 0;
my $orb_debug = 0;

my $servers_count = 2;
my $servers_kill_count = 1;
my $signalnum = 9;
my $rm2523 = 0;
my $act_delay = 800; #msec
my $start_delay = 0; #sec
my $rm_opts = "";
my $force = 0;

if ($#ARGV >= 0) {
    my $sn_set = 0;
    for (my $i = 0; $i <= $#ARGV; $i++) {
        if ($ARGV[$i] eq '-debug') {
            $orb_debug = 4;
            $imr_debug = 6;
        }
        elsif ($ARGV[$i] eq "-servers") {
            $i++;
            $servers_count = $ARGV[$i];
        }
        elsif ($ARGV[$i] eq "-servers_to_kill") {
            $i++;
            $servers_kill_count = $ARGV[$i];
        }
        elsif ($ARGV[$i] eq "-signal") {
            $i++;
            $signalnum = $ARGV[$i];
            $sn_set = 1;
        }
        elsif ($ARGV[$i] eq "-rm2523") {
            $rm2523 = 1;
            $signalnum = 15;
            $servers_count = 3;
        }
        elsif ($ARGV[$1] eq "-rm2523ol") {
            $rm2523 = 2;
            $signalnum = 15;
            $servers_count = 3;
        }
        elsif ($ARGV[$i] eq "-force") {
            $rm_opts = "-f";
            $force = 1;
        }
        elsif ($ARGV[$i] eq "-start_delay") {
            $i++;
            $start_delay = $ARGV[$i];
        }
        else {
            usage();
            exit 1;
        }
    }
    $rm_opts .= " -s $signalnum" if ($force == 1 && $sn_set == 1);
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
my $obj_count = ($rm2523 > 0) ? 1 : 2;
my $port = 9876;

my $objprefix = "TestObject";
my $client_wait_time = 10;

$imriorfile = "imr_locator.ior";
$actiorfile = "imr_activator.ior";
$imrlogfile = "imr.log";
$actlogfile = "act.log";

my $imr_imriorfile = $imr->LocalFile ($imriorfile);
my $act_imriorfile = $act->LocalFile ($imriorfile);
my $ti_imriorfile = $ti->LocalFile ($imriorfile);
my $srv_imriorfile = $srv[0]->LocalFile ($imriorfile);
my $act_actiorfile = $act->LocalFile ($actiorfile);
my $imr_imrlogfile = $imr->LocalFile ($imrlogfile);
my $act_actlogfile = $act->LocalFile ($actlogfile);

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
$imr->DeleteFile ($imrlogfile);
$act->DeleteFile ($actlogfile);

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
    my $imr_args = "-v 1000 -o $imr_imriorfile -orbendpoint iiop://localhost:$port";
    $imr_args .= " -d $imr_debug -orbdebuglevel $orb_debug -orbverboselogging 1 -orblogfile $imr_imrlogfile" if ($imr_debug > 0);
    print "$imr_args \n";
    $IMR->Arguments ($imr_args);

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

    my $act_args = "-l -o $act_actiorfile -ORBInitRef ImplRepoService=file://$act_imriorfile  -orbendpoint iiop://localhost:";
    $act_args .= " -d $imr_debug -orbdebuglevel $orb_debug -orbverboselogging 1 -orblogfile $actlogfile" if ($imr_debug > 0);
    $act_args .= " -delay $act_delay" if ($rm2523 > 0 && $OSNAME ne "MSWin32");
    $ACT->Arguments ($act_args);

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
                        "add $objprefix" . '_' . $i . "_a -c \"".
                        $srv_server_cmd[$i].
                        " -ORBUseIMR 1 -n $i -d $start_delay ".
                        "-orbendpoint iiop://localhost: " .
                        "-ORBInitRef ImplRepoService=file://$srv_imriorfile\"");

        $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
        if ($TI_status != 0) {
            print STDERR "ERROR: tao_imr returned $TI_status\n";
            $ACT->Kill (); $ACT->TimedWait (1);
            $IMR->Kill (); $IMR->TimedWait (1);
            return 1;
        }
        if ($rm2523 > 0) {
            $TI->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile ".
                            "link $objprefix" . '_' . $i . "_a " .
                            " -p $objprefix" . '_' . $i . "_b ");

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
        # For some reason the servers take forever to spawn when using the activator
        $client_wait_time *= $obj_count;

        if ($status == 1) {
            last;
        }
    }

}

sub update_manual()
{
    my $i = 1;
    $TI->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile ".
                    "update $objprefix" . '_' . $i . "_a -a MANUAL");
    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
    if ($TI_status != 0) {
        print STDERR "tao_imr update returned $TI_status\n";
    }
}

sub update_normal()
{
    my $i = 1;
    $TI->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile ".
                    "add $objprefix" . '_' . $i . "_a -c \"".
                    $srv_server_cmd[$i].
                    " -ORBUseIMR 1 -n $i ".
                    "-orbendpoint iiop://localhost: " .
                    "-ORBInitRef ImplRepoService=file://$srv_imriorfile\"");

    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
    if ($TI_status != 0) {
        print STDERR "tao_imr update returned $TI_status\n";
    }
}

sub kill_the_one()
{
    my $i = 1;
    $TI->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile ".
                    "kill $objprefix" . '_' . $i . "_a -s $signalnum");
    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
    if ($TI_status != 0) {
        print STDERR "tao_imr kill returned $TI_status\n";
    }
}

sub start_the_one()
{
    my $i = 1;
    $TI->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile ".
                    "start $objprefix" . '_' . $i . "_a");
    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
    if ($TI_status != 0) {
        print STDERR "tao_imr start returned $TI_status\n";
    }
}

sub remove_entry(@)
{
    my $obj = shift;
    my $i = 1;
    $TI->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile ".
                    "remove $objprefix" . '_' . $i . "_$obj $rm_opts");
    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
    if ($TI_status != 0) {
        print STDERR "tao_imr remove $rm_opts returned $TI_status\n";
    }
}

sub make_server_requests()
{
    print "Making requests to servers\n";

    ##### Run client against servers to active them #####
    for(my $i = 0; $i < $servers_count; $i++ ) {
	$CLI->Arguments ("-ORBInitRef Test=corbaloc::localhost:$port/$objprefix" . '_' . $i . "_a" . $debug );
	$CLI_status = $CLI->SpawnWaitKill ($cli->ProcessStartWaitInterval());
	if ($CLI_status != 0) {
	    print STDERR "ERROR: client returned $CLI_status\n";
	    $status = 1;
	}
    }
}

sub trigger_the_one ()
{
    print "Starting slow server\n";

    my $i = 1;
    my $opt_arg = " -e" if ($start_delay > 0);
    $CLI->Arguments ("-ORBInitRef Test=corbaloc::localhost:$port/$objprefix" . '_' . $i . "_a" .
        $opt_arg);
    $CLI_status = $CLI->Spawn ($cli->ProcessStartWaitInterval());
    if ($CLI_status != 0) {
        print STDERR "ERROR: client returned $CLI_status\n";
        $status = 1;
        last;
    }
}

sub wait_for_client ()
{
    print "Wait for client exit\n" ;

    $CLI_status = $CLI->WaitKill (10);
    if ($CLI_status != 0) {
        print STDERR "ERROR: client returned $CLI_status\n";
        $status = 1;
    }
}

sub shutdown_servers(@)
{
    my $start_index = shift;
    my $end_index = shift;
    my $signum = shift;
    for(my $i = $start_index; $i < $end_index; $i++ ) {
	my $status_file_name = $objprefix . "_$i.status";
        # Shutting down any server object within the server will shutdown the whole server
        $TI->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile ".
                        "kill $objprefix" . '_' . $i . "_a -s $signum" );
        $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
        if ($TI_status != 0 && $TI_status != 5) {
            print STDERR "ERROR: tao_imr kill returned $TI_status\n";
            $status = 1;
        }
	$srv[$i]->DeleteFile ($status_file_name);
    }
}

sub list_servers($)
{
    my $list_options = shift;
    print "list active\n" if ($list_options eq "-a");
    print "list registered\n" if ($list_options eq "");
    print "list verbose\n" if ($list_options eq "-v");
    $TI->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile list $list_options");
    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
    if ($TI_status != 0) {
	print STDERR "tao_imr list returned $TI_status\n";
    }
}

sub count_active_servers($)
{
    my $list_options = shift;
    my $start_time = time();
    $TI->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile list $list_options");
    # Redirect output so we can count number of lines in output
    redirect_output();
    $result = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
    my $list_time = time() - $start_time;
    restore_output();
    if ($result != 0) {
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


sub rm2523_update_test
{
    print "Running slow activator kill test with $servers_count servers \n";

    my $result = 0;
    my $start_time = time();
    servers_setup();

    print "make server requests\n";
    make_server_requests();
    list_servers("-a");

    print "kill then start the server twice\n";
    kill_the_one();
    trigger_the_one ();
    start_the_one ();
    print "pausing 2 seconds\n";
    sleep (2);
    wait_for_client ();
    shutdown_servers (0, $servers_count, 9);

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

}


sub rm2523_test
{
    print "Running slow activator kill test with $servers_count servers \n";

    my $result = 0;
    my $start_time = time();
    servers_setup();

    if ($start_delay == 0) {
        make_server_requests();
        list_servers("-a");

        if ($force == 0) {
            print "Update to manual\n";
            update_manual();
            list_servers("-v");

            print "kill the one\n";
            kill_the_one();
            list_servers("");

            print "remove primary\n";
        }
        else {
            print "force remove primary\n";
        }
        remove_entry("a");
        list_servers("");

        sleep 1;

        print "kill the one again\n";
        kill_the_one();
        list_servers("");

        print "re-add entry\n";
        update_normal();
        list_servers("");

        print "start the server\n";
        start_the_one ();
        list_servers("-a");
    }
    else {
        print "start_all - total delay " . ($servers_count * $start_delay) . " seconds\n";
        make_server_requests ();
        print "kill then list\n";
        kill_the_one ();
        list_servers("");
        sleep 2;
        print "triggering the one\n";
        trigger_the_one ();
        if ($force == 0) {
            kill_the_one ();
            list_servers("-a");
            sleep 1;
        }
        remove_entry ("a");

        wait_for_client ();
    }
    shutdown_servers (0, $servers_count, 9);

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

}

sub servers_kill_test
{
    print "Running server kill test with $servers_count servers and $obj_count objects.\n";

    my $result = 0;
    my $start_time = time();
    servers_setup();

    # Make sure servers are active whether activator is used or not by making
    # CORBA requests.
    make_server_requests();

    print "\nList of active servers before killing server(s)\n";
    $active_servers_before_kill = count_active_servers("-a");

    # Kill servers and verify listing of active servers is correct.
    print "\nKilling $servers_kill_count servers\n";

    shutdown_servers (0, $servers_kill_count, $signalnum);
    sleep (4);

    print "\nList of active servers after killing a server\n";
    $active_servers_after_kill = count_active_servers ("-a");
    if ($active_servers_after_kill != $active_servers_before_kill - $servers_kill_count) {
	print STDERR
	    "ERROR: Excepted list of active servers after killing ".
	    "a server to be " . ($active_servers_before_kill -  $servers_kill_count) .
	    " but was $active_servers_after_kill\n";
	$status = 1;
    }

    print "\n";
    shutdown_servers ($servers_kill_count, $servers_count, 9);

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
	"[-servers <num=$servers_count>] ".
	"[-servers_to_kill <num=$servers_kill_count>]\n";
}

###############################################################################
###############################################################################
my $ret = ($rm2523 > 0) ?
    ($rm2523 == 1) ? rm2523_test () : rm2523_update_test()
    : servers_kill_test();
exit $ret;
