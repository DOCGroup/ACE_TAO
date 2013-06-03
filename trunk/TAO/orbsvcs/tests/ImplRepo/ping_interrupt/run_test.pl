eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

###############################################################################
use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$imr_debug = "";

if ($#ARGV >= 0) {
    for (my $i = 0; $i <= $#ARGV; $i++) {
	if ($ARGV[$i] eq '-debug') {
            $imr_debug = "-d 5 -ORBDebugLevel 10 -ORBVerboseLogging 1 -ORBLogFile imr_loc.log";
	    $i++;
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
my $srv = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";

my $refstyle = " -ORBobjrefstyle URL";
my $obj_count = 1;
my $port = 9876;

my $objprefix = "TestObject_0";
my $client_wait_time = 10;

$imriorfile = "imr_locator.ior";
$actiorfile = "imr_activator.ior";
$persistxml = "persist.xml";
$persistdat = "persist.dat";

my $imr_imriorfile = $imr->LocalFile ($imriorfile);
my $act_imriorfile = $act->LocalFile ($imriorfile);
my $ti_imriorfile = $ti->LocalFile ($imriorfile);
my $act_actiorfile = $act->LocalFile ($actiorfile);
my $imr_persistxml = $imr->LocalFile ($persistxml);
my $imr_persistdat = $imr->LocalFile ($persistdat);

$IMR = $imr->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/tao_imr_locator");
$ACT = $act->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/tao_imr_activator");
$TI = $ti->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_imr");

# Make sure the files are gone, so we can wait on them.
$imr->DeleteFile ($imriorfile);
$act->DeleteFile ($imriorfile);
$ti->DeleteFile ($imriorfile);
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

sub server_setup ()
{
    print "initializing activator\n";

    $ACT->Arguments ("-d 0 -l -o $act_actiorfile -ORBInitRef ImplRepoService=file://$act_imriorfile");

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

    my $SRV = $srv->CreateProcess ("server");
    my $server_cmd = $SRV->Executable();
    my $srv_server_cmd = $imr->LocalFile ($server_cmd);

    ##### Add servers to activator #####
    print "adding server\n";
    my $status_file_name = $objprefix . ".status";

    $act->DeleteFile ($status_file_name);
    $TI->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile ".
                    "add $objprefix -c \"$srv_server_cmd -ORBUseIMR 1 ".
                    "-ORBInitRef ImplRepoService=file://$imr_imriorfile\"");

    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
    if ($TI_status != 0) {
        print STDERR "ERROR: tao_imr returned $TI_status\n";
        $ACT->Kill (); $ACT->TimedWait (1);
        $IMR->Kill (); $IMR->TimedWait (1);
        return 1;
    }


    $TI->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile ".
                    "start $objprefix");

    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
    if ($TI_status != 0 && $TI_status != 4) {
        print STDERR "ERROR: tao_imr returned $TI_status\n";
        $ACT->Kill (); $ACT->TimedWait (1);
        $IMR->Kill (); $IMR->TimedWait (1);
        return 1;
    }
    $TI_status = 0;
}


sub interrupt_ping_test
{
    print "Running interrupt ping test.\n";

    my $result = 0;
    my $start_time = time();

    $IMR->Arguments ("-i $imr_debug -o $imr_imriorfile -orbendpoint iiop://:$port");

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

    server_setup();

    sleep (2);

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
	"[-debug]\n";
}

###############################################################################
###############################################################################

my $ret = interrupt_ping_test();

exit $ret;
