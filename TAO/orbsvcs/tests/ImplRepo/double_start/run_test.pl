eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

###############################################################################
use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debuglevel = 0;
$cltdbg = 1;
$kill = 0;
$server_pid = 0;

if ($#ARGV >= 0) {
    for (my $i = 0; $i <= $#ARGV; $i++) {
	if ($ARGV[$i] eq '-debug') {
	    $debuglevel = 10;
	}
	elsif ($ARGV[$i] eq '-cd') {
            $i++;
	    $cltdbg = $ARGV[$i];
	}
        elsif ($ARGV[$i] eq '-kill') {
            $kill = 1;
        }
	else {
            usage();
	    exit 1;
	}
    }
}

my $tgt_num = 0;
my $imr = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
my $replica_imr = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
my $act = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
my $ti  = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
my $srv = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
my $clt = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";

my $port = 10001 + $imr->RandomPort ();
my $rport = $port + 1;

my $objprefix = "TestObject_";

my $imriorfile = "imr_locator.ior";
my $actiorfile = "imr_activator.ior";
my $primaryiorfile = "ImR_ReplicaPrimary.ior";
my $backupiorfile = "ImR_ReplicaBackup.ior";
my $srviorfile = "test.ior";

my $imr_imriorfile = $imr->LocalFile ($imriorfile);
my $act_imriorfile = $act->LocalFile ($imriorfile);
my $ti_imriorfile = $ti->LocalFile ($imriorfile);
my $srv_imriorfile = $srv->LocalFile ($imriorfile);
my $act_actiorfile = $act->LocalFile ($actiorfile);
my $srv_srviorfile = $srv->LocalFile ($srviorfile);
my $clt_srviorfile = $clt->LocalFile ($srviorfile);

my $ti_initref = "-ORBInitRef ImplRepoService=file://$ti_imriorfile";
my $act_initref = "-ORBInitRef ImplRepoService=file://$act_imriorfile";

$IMR = $imr->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/tao_imr_locator");
$RIMR = $replica_imr->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/tao_imr_locator");
$ACT = $act->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/tao_imr_activator");
$TI = $ti->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_imr");
$SRV = $srv->CreateProcess ("server");
$CLT = $clt->CreateProcess ("client");

my $server_cmd = $act->LocalFile ($SRV->Executable());

my $imrlogfile = "imr.log";
my $rimrlogfile = "replica_imr.log";
my $actlogfile = "act.log";
my $cltlogfile = "client.log";
my $srvlogfile = "server.log";

my $imr_imrlogfile = $imr->LocalFile ($imrlogfile);
my $act_actlogfile = $act->LocalFile ($actlogfile);

my $stdout_file      = "test.out";
my $stderr_file      = "test.err";
my $ti_stdout_file = $ti->LocalFile ($stdout_file);
my $ti_stderr_file = $ti->LocalFile ($stderr_file);

sub delete_files
{
    my $logs_too = shift;
    if ($logs_too == 1) {
        $imr->DeleteFile ($imrlogfile);
        $replica_imr->DeleteFile ($rimrlogfile);
        $act->DeleteFile ($actlogfile);
        $clt->DeleteFile ($cltlogfile);
        $srv->DeleteFile ($srvlogfile);
    }
    $imr->DeleteFile ($imriorfile);
    $replica_imr->DeleteFile ($replica_imriorfile);
    $act->DeleteFile ($imriorfile);
    $ti->DeleteFile ($imriorfile);
    $ti->DeleteFile ($replica_imriorfile);
    $srv->DeleteFile ($imriorfile);
    $act->DeleteFile ($actiorfile);

    $ti->DeleteFile ($stdout_file);
    $ti->DeleteFile ($stderr_file);

    cleanup_replication (".");

    # Remove any stray server status files caused by aborting services
    unlink <*.status>;
}

# Clean up after exit call
END
{
#    delete_files (0);
}


sub cleanup_replication
{
    my $dir = shift;
    if (!defined($dir)) {
        $dir = ".";
    }

    my $listings = "$dir/imr_listing.xml";
    my $fnd = 0;
    if (open FILE, "<$listings") {
        while (<FILE>) {
            if ($_ =~ /fname="([^"]+)"?/) {
                $fnd = 1;
                my $file = "$dir/$1";
                print "deleting $file\n" if ($debuglevel > 0);
                $imr->DeleteFile ($file);
                $imr->DeleteFile ($file . ".bak");
            }
        }
         close FILE;
    }

#   If the primary listings file has been corrupt then perform the
#   deletions from the backup file.

    if (!$fnd) {
       if (open FILE, "<$listings" . ".bak") {
           while (<FILE>) {
               if ($_ =~ /fname="([^"]+)"?/) {
                   my $file = "$dir/$1";
                   print "deleting $file\n" if ($debuglevel > 0);
                   $imr->DeleteFile ($file);
                   $imr->DeleteFile ($file . ".bak");
               }
           }
            close FILE;
       }
    }
    print "deleting $listings\n" if ($debuglevel > 0);
    $imr->DeleteFile ("$listings");
    $imr->DeleteFile ("$listings" . ".bak");
    $imr->DeleteFile ("$dir/$primaryiorfile");
    $imr->DeleteFile ("$dir/$backupiorfile");
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

sub kill_imr
{
    my $msg = shift;
    print STDERR "ERROR: $msg\n" if (length ($msg) > 0);
    $ACT->Kill (); $ACT->TimedWait (1);
    $IMR->Kill (); $IMR->TimedWait (1);
    $RIMR->Kill (); $RIMR->TimedWait (1);
    return 1;
}

sub kill_primary
{
    print "Killing primary ImR\n";
    $IMR->Kill (); $IMR->TimedWait (1);
}

sub get_server_pid
{
    my $pid = 0;
    open (FILE, "server.pid") or die "Can't open server.pid: $!";
    while (<FILE>) {
      $pid = $_;
    }
    close FILE;
    return $pid;
}

sub signal_server
{
    my $sig = shift;
    $server_pid = get_server_pid () if ($server_pid == 0);
    print "signal $sig to server $server_pid\n";
    kill ($sig, $server_pid);
}

sub start_imr
{
    my $all = shift;
    my $debugbase = "-ORBDebugLevel $debuglevel " .
                    "-ORBVerboseLogging 1 -ORBLogFile ";
    my $actargs = "-l -o $act_actiorfile $act_initref -ORBListenEndpoints iiop://127.0.0.1:
";

    my $imrargs = " -d $debuglevel -i -v 1000 " .
        "--directory . --primary " .
        "-ORBListenEndpoints iiop://127.0.0.1:$port";

    my $rimrargs = " -d $debuglevel -i -v 1000 -o $imr_imriorfile " .
        "--directory . --backup " .
        "-ORBListenEndpoints iiop://127.0.0.1:$rport";

    if ($debuglevel > 0) {
        $imrargs .= " $debugbase $imrlogfile";
        $rimrargs .= " $debugbase $rimrlogfile";
        $actargs .= " $debugbase $actlogfile";
    }

    print "imr args = \"$imrargs\"\n" if ($debuglevel > 0);
    print "replica imr args = \"$rimrargs\"\n" if ($debuglevel > 0);
    print "act args = \"$actargs\"\n" if ($debuglevel > 0);

    $IMR->Arguments ($imrargs);
    $RIMR->Arguments ($rimrargs);
    $ACT->Arguments ($actargs);

    ##### Start ImplRepo #####
    $IMR_status = $IMR->Spawn ();
    if ($IMR_status != 0) {
        print STDERR "ERROR: ImplRepo Service returned $IMR_status\n";
        return 1;
    }
    if ($imr->WaitForFileTimed ($primaryiorfile, $imr->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$primaryiorfile>\n";
        $IMR->Kill (); $IMR->TimedWait (1);
        return 1;
    }

    if ($all == 1) {
        $IMR_status = $RIMR->Spawn ();
        if ($IMR_status != 0) {
            print STDERR "ERROR: replica ImplRepo Service returned $IMR_status\n";
            $IMR->Kill (); $IMR->TimedWait (1);
            return 1;
        }
        if ($imr->WaitForFileTimed ($imriorfile, $imr->ProcessStartWaitInterval()) == -1) {
            print STDERR "ERROR: cannot find file <$imr_imriorfile>\n";
            $IMR->Kill (); $IMR->TimedWait (1);
            $RIMR->Kill (); $RIMR->TimedWait (1);
            return 1;
        }

        if ($imr->GetFile ($imriorfile) == -1) {
            print STDERR "ERROR: cannot retrieve file <$imr_imriorfile>\n";
            $IMR->Kill (); $IMR->TimedWait (1);
            $RIMR->Kill (); $RIMR->TimedWait (1);
            return 1;
        }
        if ($act->PutFile ($imriorfile) == -1) {
            print STDERR "ERROR: cannot set file <$act_imriorfile>\n";
            $IMR->Kill (); $IMR->TimedWait (1);
            $RIMR->Kill (); $RIMR->TimedWait (1);
            return 1;
        }
        if ($ti->PutFile ($imriorfile) == -1) {
            print STDERR "ERROR: cannot set file <$ti_imriorfile>\n";
            $IMR->Kill (); $IMR->TimedWait (1);
            $RIMR->Kill (); $RIMR->TimedWait (1);
            return 1;
        }
        if ($srv->PutFile ($imriorfile) == -1) {
            print STDERR "ERROR: cannot set file <$srv_imriorfile>\n";
            $IMR->Kill (); $IMR->TimedWait (1);
            $RIMR->Kill (); $RIMR->TimedWait (1);
            return 1;
        }

        $ACT_status = $ACT->Spawn ();
        if ($ACT_status != 0) {
            print STDERR "ERROR: ImR Activator returned $ACT_status\n";
            return 1;
        }
        if ($act->WaitForFileTimed ($actiorfile,$act->ProcessStartWaitInterval()) == -1) {
            return kill_imr ("cannot find file <$act_imriorfile>");
        }
    }
}

sub run_client
{
    if ($srv->GetFile ($srviorfile) == -1) {
        print STDERR "ERROR: cannot retrieve file <$srv_srviorfile>\n";
        return 1;
    }
    if ($clt->PutFile ($srviorfile) == -1) {
        print STDERR "ERROR: cannot set file <$clt_srviorfile>\n";
        return 1;
    }

    my $args = "-k file://$srviorfile -ORBSvcConf clt.conf";
    $args .= " -ORBDebuglevel $cltdbg -ORBVerboseLogging 1 -ORBLogFile $cltlogfile" if ($debuglevel > 0);

    print "running client $args\n";

    $CLT->Arguments ($args);
    if ($CLT->SpawnWaitKill ($clt->ProcessStartWaitInterval() + 120) == -1) {
        print STDERR "ERROR: client failed\n";
        return 1;
    }
}

sub do_ti_command
{
    my $cmd = shift;
    my $cmdargs1 = shift;
    my $cmdargs2 = shift;

    my $obj_name = $objprefix . "0";
    my $cmdargs = $cmdargs1;
    $cmdargs .= "0 $cmdargs2" if (length ($cmdargs2) > 0);
    print "invoking ti cmd $cmd $obj_name $cmdargs\n" if ($debuglevel > 0);
    $TI->Arguments ("$ti_initref $cmd $obj_name $cmdargs");
    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
    if ($TI_status != 0) {
        return kill_imr ("tao_imr $cmd $obj_name returned $TI_status");
    }
}

sub list_active_servers
{
    my $list_options = shift;
    my $start_time = time();
    $TI->Arguments ("$ti_initref list $list_options");
    # Redirect output so we can count number of lines in output
    redirect_output();
    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
    my $list_time = time() - $start_time;
    restore_output();
    if ($TI_status != 0) {
        kill_imr ("tao_imr list returned $TI_status");
        return -1;
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

sub kill_primary_test
{
    print "Running double server start test killing the primary ImR.\n";

    my $result = 0;
    my $start_time = time();

    if (start_imr (1) != 0) {
        return 1;
    }

    my $cmdline = $server_cmd . " -o $srviorfile -ORBUseIMR 1 -n 0 $act_initref "
        . "-ORBListenEndpoints iiop://127.0.0.1:";
    $cmdline = "./restart.sh -e \\\"$cmdline\\\" -p 0 -r 0 -s $objprefix" . "0";

    if (do_ti_command ("add", "-c \"$cmdline\"") != 0) {
        return 1;
    }

    if (do_ti_command ("start") != 0) {
        return 1;
    }

    list_active_servers ("-v");

    signal_server ("STOP");
    kill_primary ();
    sleep 2;
    start_imr (0);
    sleep 2;
    print "starting client\n";

    if (run_client () != 0) {
        return 1;
    }

    signal_server ("CONT");

    my $final_pid = get_server_pid ();
    if ($final_pid != $server_pid) {
        print "first server pid was $server_pid, but now there is $final_pid\n";
        $server_pid = $final_pid;
        signal_server ("TERM");
        $status = 1;
    }

    if (do_ti_command ("shutdown") != 0) {
        return 1;
    }

    kill_imr ("");

    my $test_time = time() - $start_time;

    print "\nFinished. The test took $test_time seconds.\n";

    return $status;
}

sub double_server_test
{
    print "Running double server start test.\n";

    my $result = 0;
    my $start_time = time();

    if (start_imr (1) != 0) {
        return 1;
    }

    my $cmdline = $server_cmd . " -o $srviorfile -ORBUseIMR 1 -n 0 $act_initref "
        . "-ORBListenEndpoints iiop://127.0.0.1:";
    $cmdline = "./restart.sh -e \\\"$cmdline\\\" -p 0 -r 0 -s $objprefix" . "0";

    if (do_ti_command ("add", "-c \"$cmdline\"") != 0) {
        return 1;
    }

    if (do_ti_command ("start") != 0) {
        return 1;
    }

    list_active_servers ("-v");

    if (run_client () != 0) {
        return 1;
    }

    if (do_ti_command ("shutdown") != 0) {
        return 1;
    }

    kill_imr ("");

    my $test_time = time() - $start_time;

    print "\nFinished. The test took $test_time seconds.\n";

    return $status;
}

sub usage() {
    print "Usage: run_test.pl [-debug] [-cd n]\n";
    print "       -debug enables ImR debugging\n";
    print "       -cd n sets client debug level to n (default 1)\n";
}

###############################################################################
###############################################################################

delete_files (1);

$ret = 0;
if ($kill) {
    $ret = kill_primary_test ();
}
else {
    $ret = double_server_test ();
}

exit $ret;
