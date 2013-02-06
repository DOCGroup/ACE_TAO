eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

###############################################################################
use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use Time::HiRes # Use to wait less than a second between client invocations.

$status = 0;
$debug_level = '0';

# Allow for manually launching ImpleRepo for debugging purposes.
my $no_imr = 0;

my $clients_count = 6;
my $msecs_between_clients = 500;
my $server_init_delay = 1;
my $server_reply_delay = 0;
my $rt_timeout_msecs = 0;
my $max_rt_tries = 1;

if ($#ARGV >= 0) {
    for (my $i = 0; $i <= $#ARGV; $i++) {
	if ($ARGV[$i] eq '-debug') {
	    $debug_level = '10';
	    $i++;
	}
	elsif ($ARGV[$i] eq "-clients") {
	    $i++;
	    $clients_count = $ARGV[$i];
	}
	elsif ($ARGV[$i] eq "-rt_timeout") {
	    $i++;
	    $rt_timeout_msecs = $ARGV[$i];
	}
	elsif ($ARGV[$i] eq "-msecs_between_clients") {
	    $i++;
	    $msecs_between_clients = $ARGV[$i];
	}
	elsif ($ARGV[$i] eq "-server_init_delay") {
	    $i++;
	    $server_init_delay = $ARGV[$i];
	}
	elsif ($ARGV[$i] eq "-server_reply_delay") {
	    $i++;
	    $server_reply_delay = $ARGV[$i];
	}
	elsif ($ARGV[$i] eq "-rt_timeout") {
	    $i++;
	    $rt_timeout_msecs = $ARGV[$i];
	}
	elsif ($ARGV[$i] eq "-max_rt_tries") {
	    $i++;
	    $max_rt_tries = $ARGV[$i];
	}
	elsif ($ARGV[$i] eq "-no_imr") {
	    $no_imr = 1;
	}
	else {
	    usage();
	    exit 1;
	}
    }
}

my $tgt_num = 0;
my $imr = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
my $act = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
my $ti  = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
my $srv = PerlACE::TestTarget::create_target (++$tgt_num) || die "Create target $tgt_num failed\n";
my @cli;
for(my $i = 0; $i < $clients_count; $i++) {
    push (@cli, PerlACE::TestTarget::create_target (++$tgt_num)) || die "Create target $tgt_num failed\n";
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
my $srv_imriorfile = $srv->LocalFile ($imriorfile);
my $act_actiorfile = $act->LocalFile ($actiorfile);
my $imr_persistxml = $imr->LocalFile ($persistxml);
my $imr_persistdat = $imr->LocalFile ($persistdat);

$IMR = $imr->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/tao_imr_locator");
$ACT = $act->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/tao_imr_activator");
$TI  = $ti->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_imr");
$SRV = $srv->CreateProcess ("server");
my $server_cmd = $SRV->Executable();
my $srv_server_cmd = $imr->LocalFile ($server_cmd);
@CLI;
my @srv_server_cmd;
for(my $i = 0; $i < $clients_count; $i++) {
    push (@CLI, $cli[$i]->CreateProcess ("client"));
}
# Make sure the files are gone, so we can wait on them.
if (!no_imr) {
    $imr->DeleteFile ($imriorfile);
    $act->DeleteFile ($imriorfile);
    $ti->DeleteFile ($imriorfile);
    $srv->DeleteFile ($imriorfile);
}
$act->DeleteFile ($actiorfile);
$imr->DeleteFile ($persistxml);
$imr->DeleteFile ($persistdat);

sub scale_clients_test
{
    print "Running scale_clients test with $clients_count clients.\n";

    my $result = 0;
    my $start_time = time();

    $IMR->Arguments ("-d 1 -o $imr_imriorfile -orbendpoint iiop://:$port ");
#		     "-ORBDebugLevel 10 -ORBVerboseLogging");

    if ($no_imr) {
	print STDERR "IMR assumed to be manually launched in way that is ".
	    "compatbile with:\n";
	print STDERR $IMR->CommandLine () . "\n";
    } else {

	print ">>> " . $IMR->CommandLine () . "\n";

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

    $ACT->Arguments ("-d 1 -o $act_actiorfile -ORBInitRef ImplRepoService=file://$act_imriorfile");
    print ">>> " . $ACT->CommandLine () . "\n";

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

    ##### Add server to activator #####
    my $status_file_name = $objprefix . ".status";
    my $srv_status_file = $srv->LocalFile ($status_file_name);
    $srv->DeleteFile ($srv_status_file);

    $TI->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile ".
		    "add $objprefix" . " -c \"".
		    $srv_server_cmd . " ".
		    "-ORBUseIMR 1 -d $server_init_delay -n $clients_count ".
		    "-ORBInitRef ImplRepoService=file://$imr_imriorfile\"");

    print ">>> " . $TI->CommandLine () . "\n";
    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
    if ($TI_status != 0) {
	print STDERR "ERROR: tao_imr returned $TI_status\n";
	$ACT->Kill (); $ACT->TimedWait (1);
	$IMR->Kill (); $IMR->TimedWait (1);
	return 1;
    }

    # Why is this sleep needed?
    sleep(2);

    ##### Run clients #####
    for(my $i = 0; $i < $clients_count; $i++ ) {
	# Make sure server has started by looking for its status file

	$CLI[$i]->Arguments ("-ORBInitRef Test=corbaloc::localhost:$port/$objprefix" .
			     " -d $server_reply_delay".
                             " -r $rt_timeout_msecs".
			     " -m $max_rt_tries");
	print ">>> " . $CLI[$i]->CommandLine () . "\n";
	$CLI_status = $CLI[$i]->Spawn ();
	if ($CLI_status != 0) {
	    print STDERR "ERROR: client returned $CLI_status during spawn\n";
	    $status = 1;
	    last;
	}
	Time::HiRes::usleep(1000 * $msecs_between_clients);
    }

    sleep (server_request_delay);

    ##### Stop clients #####
    print STDERR "Waiting for clients to stop\n";
    for(my $i = 0; $i < $clients_count; $i++ ) {
        my $CLI_status = $CLI[$i]->WaitKill ($cli[$i]->ProcessStartWaitInterval() +
					     $server_init_delay + $server_reply_delay);
        if ($CLI_status != 0) {
            print STDERR "ERROR: Client $i returned $CLI_status\n";
            return 1;
        }
    }

    ##### Shutdown server #####
    my $status_file_name = $objprefix . ".status";
    $srv->DeleteFile ($status_file_name);

    # Shutting down any server object within the server will shutdown the whole server
    $TI->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile ".
		    "shutdown $objprefix");
    print ">>> " . $TI->CommandLine () . "\n";
    $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
    if ($TI_status != 0) {
	print STDERR "ERROR: tao_imr shutdown returned $TI_status\n";
	$status = 1;
    }

    my $ACT_status = $ACT->TerminateWaitKill ($act->ProcessStopWaitInterval());
    if ($ACT_status != 0) {
	print STDERR "ERROR: IMR Activator returned $ACT_status\n";
	$status = 1;
    }

    if (!$no_imr) {
	my $IMR_status = $IMR->TerminateWaitKill ($imr->ProcessStopWaitInterval());
	if ($IMR_status != 0) {
	    print STDERR "ERROR: IMR returned $IMR_status\n";
	    $status = 1;
	}
    }

    my $test_time = time() - $start_time;

    print "\nFinished. The test took $test_time seconds.\n";

    return $status;
}

sub usage() {
    print "Usage: run_test.pl ".
	"[-clients <num=$clients_count>] ".
	"[-msecs_between_clients <msecs=$msecs_between_clients>] ".
	"[-server_init_delay <seconds=$server_init_delay>] ".
	"[-server_reply_delay <seconds=$server_reply_delay] ".
	"[-rt_timeout <round-trip-timeout-msecs=$rt_timeout_msecs>] ".
	"[-max_rt_tries <max-client-requests=$max_rt_tries>] ".
	"[-no_imr] ".
	"\n";
}

###############################################################################
###############################################################################

my $ret = scale_clients_test();

if (! $no_imr) {
    $imr->DeleteFile ($imriorfile);
    $act->DeleteFile ($imriorfile);
    $ti->DeleteFile ($imriorfile);
    $srv->DeleteFile ($imriorfile);
}
$act->DeleteFile ($actiorfile);
$imr->DeleteFile ($persistxml);
$imr->DeleteFile ($persistdat);

exit $ret;
