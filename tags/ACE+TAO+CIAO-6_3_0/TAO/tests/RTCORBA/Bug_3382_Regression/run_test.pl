eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-
use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

$iorbase = "test.ior";

$server_iorfile = $server->LocalFile ($iorbase);
$client_iorfile = $client->LocalFile ($iorbase);

$server->DeleteFile ($iorbase);
$client->DeleteFile ($iorbase);

my $svc_conf = 'svc.conf';

# copy the configuation files
if ($server->PutFile ($svc_conf) == -1) {
    print STDERR "ERROR: cannot set file <".$server->LocalFile ($svc_conf).">\n";
    exit 1;
}
if ($client->PutFile ($svc_conf) == -1) {
    print STDERR "ERROR: cannot set file <".$client->LocalFile ($svc_conf).">\n";
    exit 1;
}

$status = 0;

$PROC = $server->CreateProcess ("simple_client", , "-ORBSvcConfDirective \"static RT_ORB_Loader '-ORBSchedPolicy SCHED_FIFO -ORBScopePolicy SYSTEM -ORBPriorityMapping continuous'\"");

$proc_status = $PROC->SpawnWaitKill ($server->ProcessStartWaitInterval ());

if ($proc_status != 0){
    print STDERR "ERROR: simple_client returned $proc_status\n";
    $status = 1;
}

$PROC = $server->CreateProcess ("simple_client", , "-ORBSvcConfDirective \"static RT_ORB_Loader '-ORBSchedPolicy SCHED_FIFO -ORBScopePolicy SYSTEM -ORBPriorityMapping linear'\"");

$proc_status = $PROC->SpawnWaitKill ($server->ProcessStartWaitInterval ());

if ($proc_status != 0){
    print STDERR "ERROR: simple_client returned $proc_status\n";
    $status = 1;
}

print STDERR "\n            RTCORBA CLIENT_PROPAGATED Linear Priority Mapping Unit Test\n\n";

$SV = $server->CreateProcess ("server", , "-o $server_iorfile");
$CL = $client->CreateProcess ("client", "-k file://$client_iorfile");

$SV->Spawn ();

if ($server->WaitForFileTimed ($iorbase,
                              $server->ProcessStartWaitInterval()) == -1){
    $server_status = $SV->TimedWait (1);
    if ($server_status == 2){
        # Mark as no longer running to avoid errors on exit.
        $SV->{RUNNING} = 0;
        exit $status;
    }
    else{
        print STDERR "ERROR: cannot find file <$server_iorfile>\n";
        $SV->Kill ();
        exit 1;
    }
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval ());

if ($client_status != 0){
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval ());

if ($server_status != 0){
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile ($iorbase);
$client->DeleteFile ($iorbase);

exit $status;
