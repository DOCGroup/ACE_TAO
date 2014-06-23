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

$client_count = 3;
$unlinked_test = 0;

if ($#ARGV >= 0) {
    for (my $i = 0; $i <= $#ARGV; $i++) {
	if ($ARGV[$i] eq '-debug') {
	    $debug_level = 10;
	}
        elsif ($ARGV[$i] eq '-nolink') {
            $unlinked_test = 1;
        }
	else {
	    usage();
	    exit 1;
	}
    }
}

my $objprefix = "TestObject";
$statusfile = "TestObject.status";

my $tgt_num = 0;
my $imr = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
my $act = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
my $srv = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
my $ti  = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";

my @cli;
my @CLI;
my @srviorfile;
my $poa_id = 'a';
for(my $i = 0; $i < $client_count; $i++) {
    push (@cli, PerlACE::TestTarget::create_target (++$tgt_num)) || die "Create target $tgt_num failed\n";
    my $fname = $objprefix . "_" . $poa_id . ".ior";
    push (@srviorfile, $srv->LocalFile ($fname));
    push (@CLI, $cli[$i]->CreateProcess ("client"));
    $CLI[$i]->Arguments ("-" . $poa_id);
    $poa_id++;
}

my $refstyle = " -ORBobjrefstyle URL";

my $client_wait_time = 10;

$imriorfile = "imr_locator.ior";
$actiorfile = "imr_activator.ior";

my $imr_imriorfile = $imr->LocalFile ($imriorfile);
my $act_imriorfile = $act->LocalFile ($imriorfile);
my $act_actiorfile = $act->LocalFile ($actiorfile);
my $ti_imriorfile = $ti->LocalFile ($imriorfile);
$SRV = $srv->CreateProcess ("server");
$server_cmd = $SRV->Executable();
$srv_server_cmd = $imr->LocalFile ($server_cmd);

$IMR = $imr->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/tao_imr_locator");
$ACT = $act->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/tao_imr_activator");
$TI = $ti->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_imr");

$initref = "-ORBInitRef ImplRepoService=file://$ti_imriorfile";

my $stdout_file      = "test.out";
my $stderr_file      = "test.err";
my $ti_stdout_file = $ti->LocalFile ($stdout_file);
my $ti_stderr_file = $ti->LocalFile ($stderr_file);

sub delete_files
{
    $imr->DeleteFile ($imriorfile);
    $act->DeleteFile ($imriorfile);
    $act->DeleteFile ($actiorfile);
    $ti->DeleteFile ($imriorfile);
    $srv->DeleteFile ($imriorfile);
    $srv->DeleteFile ($statusfile);

    for ($i = 0; $i < $client_count; $i++) {
        $cli[$i]->DeleteFile ($imriorfile);
        $srv->DeleteFile ($srviorfile[$i]);
    }
}

# Clean up after exit call
END
{
    delete_files ();

    $ti->DeleteFile ($stdout_file);
    $ti->DeleteFile ($stderr_file);
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

sub validate_and_kill
{
    open FILE, "<$statusfile";
    @pids = <FILE>;
    $count = scalar @pids;
    print "$statusfile has $count lines\n";

    if ($unlinked_test == 1) {
        print STDERR "ERROR: expected > 1 server pids, got 1\n" if ($count == 1);
        $result = 1 if ($count == 1);
    }
    else {
        print STDERR "ERROR: expected 1 server pid, got $count\n" if ($count != 1);
        $result = 1 if ($count != 1);
    }
    for ($i = 0; $i < $count; $i++) {
        $ks = kill $pids[$i];
    }
}

sub server_setup
{
    print "Adding base server\n";

    $TI->Arguments ("$initref add $objprefix" . "_a -c \"".
                    $srv_server_cmd . " -ORBUseIMR 1 -w $initref\"");


    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
    if ($TI_status != 0) {
        print STDERR "ERROR: tao_imr returned $TI_status\n";
        $ACT->Kill (); $ACT->TimedWait (1);
        $IMR->Kill (); $IMR->TimedWait (1);
        return 1;
    }
    return 0;
}

sub list_servers
{
    print "calling list -v\n";
    $TI->Arguments ("$initref list -v");

    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
    if ($TI_status != 0) {
        print STDERR "ERROR: tao_imr returned $TI_status\n";
        $ACT->Kill (); $ACT->TimedWait (1);
        $IMR->Kill (); $IMR->TimedWait (1);
        return 1;
    }
    return 0;
}

sub link_poas
{
    print "linking POAs\n";
    $TI->Arguments ("$initref update $objprefix" . "_a -c \"".
                    $srv_server_cmd . " -ORBUseIMR 1 -d 2 $initref\"");


    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
    if ($TI_status != 0) {
        print STDERR "ERROR: tao_imr returned $TI_status\n";
        $ACT->Kill (); $ACT->TimedWait (1);
        $IMR->Kill (); $IMR->TimedWait (1);
        return 1;
    }

    $TI->Arguments ("$initref link $objprefix" . "_a -p ".
                    $objprefix . "_b," . $objprefix . "_c");

    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
    if ($TI_status != 0) {
        print STDERR "ERROR: tao_imr returned $TI_status\n";
        $ACT->Kill (); $ACT->TimedWait (1);
        $IMR->Kill (); $IMR->TimedWait (1);
        return 1;
    }
    return 0;
}

sub add_unlinked_poas
{
    print "adding unlinked POAs\n";

    $TI->Arguments ("$initref update $objprefix" . "_a -c \"".
                    $srv_server_cmd . " -ORBUseIMR 1 -d 2 $initref\"");
    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
    if ($TI_status != 0) {
        print STDERR "ERROR: tao_imr returned $TI_status\n";
        $ACT->Kill (); $ACT->TimedWait (1);
        $IMR->Kill (); $IMR->TimedWait (1);
        return 1;
    }

    my $id = 'b';
    for (my $i = 1; $i < $client_count; $i++) {
        $TI->Arguments ("$initref add $objprefix" . "_$id -c \"".
                        $srv_server_cmd . " -ORBUseIMR 1 -d 2 $initref\"");
        $id++;
        $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
        if ($TI_status != 0) {
            print STDERR "ERROR: tao_imr returned $TI_status adding unlinked POA $id\n";
            $ACT->Kill (); $ACT->TimedWait (1);
            $IMR->Kill (); $IMR->TimedWait (1);
            return 1;
        }
    }
    return 0;
}

sub shutdown_server
{
    $TI->Arguments ("$initref shutdown $objprefix" . "_a");

    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
    if ($TI_status != 0) {
        print STDERR "ERROR: tao_imr returned $TI_status\n";
        $ACT->Kill (); $ACT->TimedWait (1);
        $IMR->Kill (); $IMR->TimedWait (1);
        return 1;
    }
}

sub init_ior_files
{
    print "init ior files, starting server\n";
    $TI->Arguments ("$initref ".
                    "start $objprefix" . "_a");

    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
    if ($TI_status != 0) {
        print STDERR "ERROR: tao_imr returned $TI_status\n";
        $ACT->Kill (); $ACT->TimedWait (1);
        $IMR->Kill (); $IMR->TimedWait (1);
        return 1;
    }

    print "init ior files, shutdown server\n";
    shutdown_server ();

    my $id = 'b';
    for (my $i = 1; $i < $client_count; $i++) {
        print "init ior files, remove $id\n";
        $TI->Arguments ("$initref remove $objprefix" . "_$id");
        $id++;
        $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
        if ($TI_status != 0) {
            print STDERR "ERROR: tao_imr returned $TI_status\n";
            $ACT->Kill (); $ACT->TimedWait (1);
            $IMR->Kill (); $IMR->TimedWait (1);
            return 1;
        }
    }

    return 0;
}

sub run_clients
{
    print "running concurrent clients\n";
    for (my $i = 0; $i < $client_count; $i++)
    {
        my $cli_status = $CLI[$i]->Spawn ();
        if ($cli_status != 0) {
            print STDERR "ERROR: client $i returned $cli_status\n";
            $status = 1;
        }
    }
    for (my $i = 0; $i < $client_count; $i++)
    {
        print "waiting for $i\n";
        $status += $CLI[$i]->WaitKill ($cli[$i]->ProcessStopWaitInterval());
    }
    return 0;
}


sub servers_link_test
{
#tao_imr_locator -o imr.ior -x persist.xml
#tao_imr_activator -l -ORBInitRef ImplRepoService=file://imr.ior&
#tao_imr add TestObject_a -c "./server -ORBUseIMR 1 -d 1 -ORBInitRef ImplRepoService=file://$ti_imriorfileimr.ior"
#tao_imr link TestObject_a -p TestObject_b,TestObject_c
#tao_imr start TestObject_a
#tao_imr list -v
#tao_imr shutdown TestObject_a
#client -a & client -b & client -c
# verify return values match
#tao_imr shutdown TestObject_a
#kill activator
#kill locator
#cleanup files

    print "Running server kill test with $servers_count servers and $obj_count objects.\n";

    my $result = 0;

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
    if ($srv->PutFile ($imriorfile) == -1) {
        print STDERR "ERROR: cannot set file <$srv_imriorfile>\n";
        $IMR->Kill (); $IMR->TimedWait (1);
        return 1;
    }

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


    return 1 if (server_setup () != 0);
    return 1 if (init_ior_files () != 0);

    list_servers();

    if ($unlinked_test == 0) {
        return 1 if (link_poas () != 0);
    }
    else {
        return 1 if (add_unlinked_poas () != 0);
    }

    list_servers();

    return 1 if (run_clients () != 0);

    print "shutdown server\n";
    shutdown_server ();

    $status += validate_and_kill ();
}

sub imr_shutdown
{
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

    return $status;
}

sub usage() {
    print "Usage: run_test.pl\n";
}

###############################################################################
###############################################################################

# Make sure the files are gone, so we can wait on them.
delete_files();

my $start_time = time();
my $ret = servers_link_test();
imr_shutdown ();
my $test_time = time() - $start_time;
print "\nFinished. The test took $test_time seconds.\n";


exit $ret;
