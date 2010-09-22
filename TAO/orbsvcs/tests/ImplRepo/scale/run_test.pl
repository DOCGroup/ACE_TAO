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

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $imr = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $act = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $ti = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $srv = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $cli = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";

my $refstyle = " -ORBobjrefstyle URL";
my $servers_count = 1;
my $obj_count = 1;
my $use_activator = 0;
my $port = 9876;

my $objprefix = "TstObj";
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
$ACT = $act->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/ImR_Activator");
$TI = $ti->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_imr");
$SRV = $srv->CreateProcess ("server");
$CLI = $cli->CreateProcess ("client");

my $server_cmd = $SRV->Executable();
my $srv_server_cmd = $imr->LocalFile ($server_cmd);

# Make sure the files are gone, so we can wait on them.
$imr->DeleteFile ($imriorfile);
$act->DeleteFile ($imriorfile);
$ti->DeleteFile ($imriorfile);
$srv->DeleteFile ($imriorfile);
$act->DeleteFile ($actiorfile);
$imr->DeleteFile ($persistxml);
$imr->DeleteFile ($persistdat);

sub scale_test
{
    print "Running scale test with $servers_count servers and $obj_count objects.\n";

    my $result = 0;
    my $start_time = time();

    $IMR->Arguments ("-d 1 -o $imr_imriorfile -orbendpoint iiop://:$port");
    $IMR_status = $IMR->Spawn ();
    if ($IMR_status != 0) {
        print STDERR "ERROR: ImplRepo Service returned $IMR_status\n";
        return 1;
    }
    if ($imr->WaitForFileTimed ($imriorfile,$imr->ProcessStartWaitInterval()) == -1) {
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

    if ($use_activator) {

        $ACT->Arguments ("-d 1 -o $act_actiorfile -ORBInitRef ImplRepoService=file://$act_imriorfile");
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

        for(my $i = 0; $i < $servers_count; $i++) {
            for (my $j = 0; $j < $obj_count; $j++) {
                $TI->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile ".
                                "add $objprefix" . '_' . $i . "_" . $j . " -c \"".
                                $srv_server_cmd.
                                " -ORBUseIMR 1 -p $objprefix" . '_' . "$i -c $obj_count ".
                                "-ORBInitRef ImplRepoService=file://$imr_imriorfile\"");

                $TI_status = $TI->Spawn ();
                if ($TI_status != 0) {
                    print STDERR "ERROR: tao_imr returned $TI_status\n";
                    $ACT->Kill (); $ACT->TimedWait (1);
                    $IMR->Kill (); $IMR->TimedWait (1);
                    return 1;
                }
            }
        }

        $TI->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile list");
        $result = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
        if ($TI_status != 0) {
            print STDERR "ERROR: tao_imr returned $TI_status\n";
            $ACT->Kill (); $ACT->TimedWait (1);
            $IMR->Kill (); $IMR->TimedWait (1);
            return 1;
        }
    }

    for(my $i = 0; $i < $servers_count; $i++ ) {

        my $startfile = $objprefix . "_$i.status";
        my $srv_startfile = $srv->LocalFile ($startfile);
        $srv->DeleteFile ($objprefix . "_$i.status");

        if (! $use_activator) {
            $SRV->Arguments ("-ORBUseIMR 1 -p $objprefix" . '_' . "$i -c $obj_count -ORBInitRef ".
                             "ImplRepoService=file://$srv_imriorfile");
            $SRV_status = $SRV->Spawn ();
            if ($SRV_status != 0) {
                print STDERR "ERROR: server returned $SRV_status\n";
                $SRV->Kill (); $SRV->TimedWait (1);
                $ACT->Kill (); $ACT->TimedWait (1);
                $IMR->Kill (); $IMR->TimedWait (1);
                return 1;
            }
            if ($srv->WaitForFileTimed ($startfile,$srv->ProcessStartWaitInterval()) == -1) {
                print STDERR "ERROR: cannot find file <$srv_startfile>\n";
                $SRV->Kill (); $SRV->TimedWait (1);
                $ACT->Kill (); $ACT->TimedWait (1);
                $IMR->Kill (); $IMR->TimedWait (1);
                return 1;
            }
        } else {
            # For some reason the servers take forever to spawn when using the activator
            $client_wait_time *= $obj_count;
        }
        $srv->DeleteFile ($objprefix . "_$i.status");

        for (my $j = 0; $j < $obj_count; $j++) {
            $CLI->Arguments ("-orbinitref Test=corbaloc::localhost:$port/$objprefix" . '_' . $i . '_' . $j);
            $CLI_status = $CLI->SpawnWaitKill ($client_wait_time);
            if ($CLI_status != 0) {
                print STDERR "ERROR: client returned $CLI_status\n";
                $status = 1;
                last;
            }
        }

        # Shutting down any server object within the server will shutdown the whole server
        $TI->Arguments ("-ORBInitRef ImplRepoService=file://$ti_imriorfile ".
                        "shutdown $objprefix" . '_' . $i . "_0");
        $TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
        if ($TI_status != 0) {
            print STDERR "ERROR: tao_imr shutdown returned $TI_status\n";
            $status = 1;
            last;
        }
        if (! $use_activator) {
            $SRV_status = $SRV->WaitKill($srv->ProcessStopWaitInterval());
            if ($SRV_status != 0) {
                print STDERR "ERROR: server not shutdown correctly. It returned $SRV_status\n";
                $status = 1;
                last;
            }
        }

        if ($status == 1) {
            last;
        }
    }

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
    my $total_objs = $obj_count * $servers_count;

    print "\nFinished. The test took $test_time seconds for $total_objs imr-ified objects.\n";

    return $status;
}

sub usage() {
    print "Usage: run_test.pl [-servers <num=1>] [-objects <num=1>] [-use_activator]\n";
}

###############################################################################
###############################################################################

if ($#ARGV >= 0) {
    for (my $i = 0; $i <= $#ARGV; $i++) {
        if ($ARGV[$i] eq "-servers") {
            $i++;
            $servers_count = $ARGV[$i];
        }
        elsif ($ARGV[$i] eq "-objects") {
            $i++;
            $obj_count = $ARGV[$i];
        }
        elsif ($ARGV[$i] eq "-use_activator") {
            $use_activator = 1;
        }
        else {
            usage();
            exit 1;
        }
    }
}

my $ret = scale_test();

$imr->DeleteFile ($imriorfile);
$act->DeleteFile ($imriorfile);
$ti->DeleteFile ($imriorfile);
$srv->DeleteFile ($imriorfile);
$act->DeleteFile ($actiorfile);
$imr->DeleteFile ($persistxml);
$imr->DeleteFile ($persistdat);

exit $ret;
