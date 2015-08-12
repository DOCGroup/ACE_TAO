eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-

###############################################################################
use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $status = 0;
my $debuglevel = 0;
my $srv_count = 48;
my $thr_count = 1;
my $no_imr = 0;
my $cl_only = 0;
my $port;
my $rport;
my $nsport;

for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-debug") {
        print "debug mode\n";
        $debuglevel = 4;
    }
    elsif ($ARGV[$i] eq "--servers" || $ARGV[$i] eq "-s") {
        $srv_count = $ARGV[++$i];
    }
    elsif ($ARGV[$i] eq "--threads" || $ARGV[$i] eq "-t") {
        $thr_count = $ARGV[++$i];
    }
    elsif ($ARGV[$i] eq "--clonly" || $ARGV[$i] eq "-c") {
        $cl_only = $ARGV[++$i];
    }
    elsif ($ARGV[$i] eq "--noimr" || $ARGV[$i] eq "-n") {
        $no_imr = 1;
        @ports = split (',', $ARGV[++$i]);
        $port = $ports[0];
        $rport = $ports[1];
        $nsport = $ports[2];
    }
    else {
        print "$ARGV[$i] is invalid\n";
        usage();
        exit 1;
    }
}

my $clt_count = $srv_count/3;
my $objprefix = "TestObject_";

my @srvdeps;
for ($i = 0; $i < $srv_count; $i++) {
    my $ofs = $i + $clt_count;
    $srvdeps[$i] = "";
    $srvdeps[$i] = $objprefix . $ofs if ($i < $clt_count * 2);
}

my $tgt_num = 0;
my $imr = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
my $replica_imr = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
my $act = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
my $nam = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
my $srv = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
my $nsl = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";

my @ti;
for ($i = 0; $i <= $srv_count; $i++) {
    $ti[$i]  = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
}

my @clt;
my @CLT;
for ($i = 0; $i < $clt_count; $i++) {
    $clt[$i] = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
    $CLT[$i] = $clt[$i]->CreateProcess ("client");
}

if (!$no_imr) {
    $port = 10001 + $imr->RandomPort ();
    $rport = $port + 1;
    $nsport = $rport + 1;
}

my $imriorfile = "imr_locator.ior";
my $actiorfile = "imr_activator.ior";
my $primaryiorfile = "ImR_ReplicaPrimary.ior";
my $backupiorfile = "ImR_ReplicaBackup.ior";

my $imr_imriorfile = $imr->LocalFile ($imriorfile);
my $act_imriorfile = $act->LocalFile ($imriorfile);
my $ti_imriorfile = $ti[0]->LocalFile ($imriorfile);
my $act_actiorfile = $act->LocalFile ($actiorfile);

my $ti_initref = "-ORBInitRef ImplRepoService=file://$ti_imriorfile";
my $act_initref = "-ORBInitRef ImplRepoService=file://$act_imriorfile";
my $nam_initref = "-ORBInitRef NameService=corbaloc::localhost:$nsport/";
my $imr_root = "$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service";
my $nam_root = "$ENV{TAO_ROOT}/orbsvcs/Naming_Service";
my $util_root = "$ENV{TAO_ROOT}/utils";

my $server_cmd = $act->LocalFile ("server");

my $IMR = $imr->CreateProcess ($imr_root . "/tao_imr_locator");
my $RIMR = $replica_imr->CreateProcess ($imr_root . "/tao_imr_locator");
my $ACT = $act->CreateProcess ($imr_root . "/tao_imr_activator");
my $NAM = $nam->CreateProcess ($nam_root . "/tao_cosnaming");
my $NSL = $nsl->CreateProcess ($util_root . "/nslist/tao_nslist");

my @TI;
for ($i = 0; $i <= $srv_count; $i++) {
    $TI[$i] = $ti[$i]->CreateProcess ($imr_root . "/tao_imr");
}

my $imrlogfile = "imr.log";
my $rimrlogfile = "replica_imr.log";
my $actlogfile = "act.log";
my $imr_imrlogfile = $imr->LocalFile ($imrlogfile);
my $act_actlogfile = $act->LocalFile ($actlogfile);

my $stdout_file      = "test.out";
my $stderr_file      = "test.err";
my $ti_stdout_file = $ti[0]->LocalFile ($stdout_file);
my $ti_stderr_file = $ti[0]->LocalFile ($stderr_file);

sub delete_files
{
    my $logs_too = shift;
    if ($logs_too == 1) {
   #     $imr->DeleteFile ($imrlogfile);
   #     $replica_imr->DeleteFile ($rimrlogfile);
        #     $act->DeleteFile ($actlogfile);
        unlink <*.log>;
    }
    $imr->DeleteFile ($imriorfile);
    $replica_imr->DeleteFile ($replica_imriorfile);
    $act->DeleteFile ($imriorfile);
    $ti[0]->DeleteFile ($imriorfile);
    $act->DeleteFile ($actiorfile);

    $ti[0]->DeleteFile ($stdout_file);
    $ti[0]->DeleteFile ($stderr_file);

    cleanup_replication (".");

    # Remove any stray server status files caused by aborting services
    unlink <*.status>;
}

# Clean up after exit call
END
{
    delete_files (0) if (!$no_imr);
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

my $level = 0;

sub kill_imr
{
    my $msg = shift;
    print STDERR "ERROR: $msg\n" if (length ($msg) > 0);
    if ($no_imr || $level == 0) {
        return 1;
    }
    if ($level > 0) {
        $NAM->TerminateWaitKill (5);
    }
    if ($level > 1) {
        $IMR->TerminateWaitKill (5);
    }
    if ($level > 2) {
        $RIMR->TerminateWaitKill (5);
    }
    if ($level > 3) {
        $ACT->TerminateWaitKill (5);
    }
    return 1;
}

sub start_imr
{
    delete_files (1) if (!$no_imr);
    my $debugbase = " -ORBDebugLevel $debuglevel " .
                    "-ORBVerboseLogging 1 -ORBLogFile ";
    my $actargs = "-l -o $act_actiorfile $act_initref " .
        "-ORBListenEndpoints iiop://localhost:";

    my $imrargs = " -i -v 1000 -d 10 --threads $thr_count " .
        "--directory . --primary " .
        "-ORBListenEndpoints iiop://localhost:$port";

    my $rimrargs = " -i -v 1000 -d 10 --threads $thr_count -o $imr_imriorfile " .
        "--directory . --backup " .
        "-ORBListenEndpoints iiop://localhost:$rport";

    my $namargs = "-ORBListenEndpoints iiop://localhost:$nsport";

    $imrargs .= " $debugbase $imrlogfile";
    $rimrargs .= " $debugbase $rimrlogfile";
    if ($debuglevel > 0) {
        $actargs .= " $debugbase $actlogfile";
    }

    if ($debuglevel > 0 || $cl_only != 0) {
        print "naming args = \"$namargs\"\n";
        print "imr args = \"$imrargs\"\n";
        print "replica imr args = \"$rimrargs\"\n";
        print "act args = \"$actargs\"\n";
        if ($cl_only != 0) {
            exit (0);
        }
    }
    $IMR->Arguments ($imrargs);
    $RIMR->Arguments ($rimrargs);
    $ACT->Arguments ($actargs);
    $NAM->Arguments ($namargs);

    if ($no_imr) {
        return 0;
    }

    ##### Start Name service #####
    my $IMR_status = $NAM->Spawn ();
    if ($IMR_status != 0) {
        return kill_imr ("NameService returned $IMR_status");
    }
    $level++;

    ##### Start ImplRepo #####
    $IMR_status = $IMR->Spawn ();
    if ($IMR_status != 0) {
        return kill_imr ("ImplRepo Service returned $IMR_status");
    }
    $level++;

    if ($imr->WaitForFileTimed ($primaryiorfile, $imr->ProcessStartWaitInterval()) == -1) {
        return kill_imr ("cannot find file <$primaryiorfile>");
    }

    $IMR_status = $RIMR->Spawn ();
    if ($IMR_status != 0) {
        return kill_imr ("replica ImplRepo Service returned $IMR_status");
    }
    $level++;

    if ($imr->WaitForFileTimed ($imriorfile, $imr->ProcessStartWaitInterval()) == -1) {
        return kill_imr ("cannot find file <$imr_imriorfile>");
    }

    if ($imr->GetFile ($imriorfile) == -1) {
        return kill_imr ("cannot retrieve file <$imr_imriorfile>");
    }
    if ($act->PutFile ($imriorfile) == -1) {
        return kill_imr ("cannot set file <$act_imriorfile>");
    }
    if ($ti[0]->PutFile ($imriorfile) == -1) {
        return kill_imr ("cannot set file <$ti_imriorfile>");
    }

    $ACT_status = $ACT->Spawn ();
    if ($ACT_status != 0) {
        return kill_imr ("ImR Activator returned $ACT_status");
    }
    $level++;

    if ($act->WaitForFileTimed ($actiorfile,$act->ProcessStartWaitInterval()) == -1) {
        return kill_imr ("cannot find file <$act_imriorfile>");
    }
    return 0;
}

sub do_ti_command
{
    my $start = shift;
    my $end = shift;
    my $delstat = shift;
    my $cmd = shift;
    my $cmdargs1 = shift;
    my $cmdargs2 = shift;
    my $cmdargs3 = shift;

    for(my $i = $start; $i < $end; $i++) {
        my $obj_name = $objprefix . "$i";
        if ($delstat != 0) {
            my $status_file_name = $obj_name . ".status";
            $srv->DeleteFile ($status_file_name);
        }
        my $cmdargs = $cmdargs1;
        if (length ($cmdargs2) > 0) {
            $cmdargs .= $i;
            $cmdargs .= "$cmdargs2 $srvdeps[$i]" if (length ($srvdeps[$i]) > 0);
            $cmdargs .= $cmdargs3;
        }
        print "invoking ti cmd $cmd $obj_name $cmdargs\n" if ($debuglevel > 0);
        $TI[$i]->Arguments ("$ti_initref $cmd $obj_name $cmdargs");
        $TI_status = $TI[$i]->Spawn ();
        if ($TI_status != 0) {
            return kill_imr ("tao_imr $cmd $obj_name returned $TI_status");
        }
    }
    for ($i = $start; $i < $end; $i++) {
        $TI[$i]->WaitKill (15);
    }

}

sub list_active_servers
{
    $ti[0]->DeleteFile ($stdout_file);
    $ti[0]->DeleteFile ($stderr_file);

    my $list_options = shift;
    my $start_time = time();
    $TI[$srv_count]->Arguments ("$ti_initref list $list_options");
    # Redirect output so we can count number of lines in output
    redirect_output();
    $TI_status = $TI[$srv_count]->SpawnWaitKill ($ti[$srv_count]->ProcessStartWaitInterval());
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

sub kill_active_servers
{
    $ti[0]->DeleteFile ($stdout_file);
    $ti[0]->DeleteFile ($stderr_file);

    $TI[$srv_count]->Arguments ("$ti_initref list -a -t");
    # Redirect output so we can count number of lines in output
    redirect_output();
    $TI_status = $TI[$srv_count]->SpawnWaitKill ($ti[$srv_count]->ProcessStartWaitInterval());
    restore_output();
    if ($TI_status != 0) {
        kill_imr ("tao_imr list returned $TI_status");
    }
    open (FILE, $stderr_file) or die "Can't open $stderr_file: $!";
    $active_servers = 0;
    while (<FILE>) {
	print STDERR "force killing $_";
        $TI[$srv_count]->Arguments ("$ti_initref kill $_");
        $TI_status = $TI[$srv_count]->SpawnWaitKill ($ti[$srv_count]->ProcessStartWaitInterval());
        if ($TI_status != 0) {
            kill_imr ("tao_imr kill returned $TI_status");
        }
    }
    close FILE;
}

sub wait_for_servers
{
    print "waiting for servers to exit\n";
    my $running = 1;
    my $retries = 5;
    while ($running > 0 && $retries > 0) {
        sleep 2;
        $running = list_active_servers ("-a");
        $retries--;
    }
    if ($running > 0) {
        kill_active_servers ();
    }
}

sub scale_test
{
    print "Running scale test with $srv_count servers and $clt_count clients.\n";

    my $result = 0;
    my $start_time = time();

    if (start_imr () != 0)
    {
        exit;
    }

    print "Adding servers\n";
    do_ti_command (0, $srv_count, 1, "add",
                   " -q -c \"$server_cmd -ORBUseIMR 1 -ORBVerboseLogging 1 -s $objprefix",
                   " -d ",
                   " -ORBListenEndpoints iiop://localhost: $act_initref $nam_initref\"");
    print "Initializing name service\n";
    do_ti_command (0, $srv_count, 1, "start", "-q", "", "");
    wait_for_servers ();

    $NSL->Arguments (" $nam_initref");
    $NSL->SpawnWaitKill (150);

    print "Running clients\n";
    for (my $i = 0; $i < $clt_count; $i++) {
        $CLT[$i]->Arguments ($nam_initref . " -s $objprefix$i  -ORBVerboseLogging 1 ");
        $CLT_status = $CLT[$i]->Spawn ();
        if ($CLT_status != 0) {
            print STDERR "ERROR: client $i spawn returned $CLT_status\n";
            $status = 1;
            last;
        }
    }

    print "Waiting on clients\n";
    for (my $i = 0; $i < $clt_count; $i++) {
        $CLT_status = $CLT[$i]->WaitKill (30);
        if ($CLT_status != 0) {
            print STDERR "ERROR: client $i waitkill returned $CLI_status\n";
            $status = 1;
        }
    }

    print "Stopping servers\n";
    do_ti_command (0, $srv_count, 1, "shutdown", "-q" );
    wait_for_servers ();

    my $test_time = time() - $start_time;
    my $total_objs = $srv_count;

    print "\nFinished. The test took $test_time seconds for $total_objs imr-ified objects.\n";
#    sleep (300);
    kill_imr ();
    return $status;
}

sub usage() {
    print "Usage: run_test.pl [-servers <num=1>] [-clients <num=1>]\n";
}

###############################################################################
###############################################################################

my $ret = scale_test();

exit $ret;
