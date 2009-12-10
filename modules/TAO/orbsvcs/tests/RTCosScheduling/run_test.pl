eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-
# $Id$

use lib  "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $client1 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $client2 = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $client3 = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";

my $srv_ior = "server.ior";
my $schedule_cfg = "schedule.cfg";
my $invalid_cfg  = "INVALID_FILE.cfg";

my $server_schedule_cfg = $server->LocalFile ($schedule_cfg);
my $server_invalid_cfg  = $server->LocalFile ($invalid_cfg);
my $server_srv_ior = $server->LocalFile ($srv_ior);
$server->DeleteFile ($srv_ior);

my $client_schedule_cfg = $client->LocalFile ($schedule_cfg);
my $client_invalid_cfg  = $client->LocalFile ($invalid_cfg);

my $client1_schedule_cfg = $client1->LocalFile ($schedule_cfg);
my $client2_schedule_cfg = $client2->LocalFile ($schedule_cfg);
my $client3_schedule_cfg = $client3->LocalFile ($schedule_cfg);

my $client1_output = $client1->LocalFile ("client1_output");
my $client2_output = $client2->LocalFile ("client2_output");
my $client3_output = $client3->LocalFile ("client3_output");
$client1->DeleteFile ($client1_output);
$client2->DeleteFile ($client2_output);
$client3->DeleteFile ($client3_output);

#S11 uses schedule object, S10 does not
$S11 = $server->CreateProcess ("server", "-N 2 -F $server_schedule_cfg -A Server2 -X 1");
$S10 = $server->CreateProcess ("server", "-N 2 -F $server_schedule_cfg -A Server2 -X 0");
#S2 uses and invalid config file
$S2  = $server->CreateProcess ("server", "-N 2 -F $server_invalid_cfg -A Server2 -X 1");
#s3 tries to schedule an invalid object
$S3  = $server->CreateProcess ("server", "-N 2 -F $server_schedule_cfg -A Server5 -X 1");

# C11 C21 C31 use RTCosScheduler 1.0 on the client
$C11 = $client1->CreateProcess ("client", "-B 0 -R 10 -A 3 -N 1 -C $client1_output ".
                                          "-F $client1_schedule_cfg -T Client1 -X 1");
$C21 = $client2->CreateProcess ("client", "-B 0 -R 3 -A 3 -N 1 -C $client2_output ".
                                          "-F $client2_schedule_cfg -T Client2 -X 1");
$C31 = $client3->CreateProcess ("client", "-B 2 -R 3 -A 3 -N 1 -C $client3_output ".
                                          "-F $client3_schedule_cfg -T Client3 -X 1");

#C10 C20 C30 Do not use RTCosScheduler 1.0 on the client
$C10 = $client1->CreateProcess ("client", "-B 0 -R 10 -A 3 -N 1 -C $client1_output ".
                                          "-F $client1_schedule_cfg -T Client1 -X 0");
$C20 = $client2->CreateProcess ("client", "-B 0 -R 3 -A 3 -N 1 -C $client2_output ".
                                          "-F $client2_schedule_cfg -T Client2 -X 0");
$C30 = $client3->CreateProcess ("client", "-B 2 -R 3 -A 3 -N 1 -C $client3_output ".
                                          "-F $client3_schedule_cfg -T Client3 -X 0");

#C4 is an invalid activity name
$C4 = $client->CreateProcess ("client","-B 1 -R 3 -A 0 -N 1 -F $client_schedule_cfg -T Client4 -X 1");
#C5 is an invalid config file
$C5 = $client->CreateProcess ("client", "-B 1 -R 3 -A 0 -N 1 -F $client_invalid_cfg -T Client2 -X 1");

sub spawn_server {
    local ( $param_1 ) = @_;
    $result = $param_1->Spawn ();
    if ($result != 0) {
        print STDERR "ERROR: server Spawn retured $result\n";
        exit 1;
    }

    if ($server->WaitForFileTimed ($srv_ior, $server->ProcesStartWaitInterval() ) == -1) {
        print STDERR "ERROR: cannot find file <$server_srv_ior>\n";
        $param_1->Kill ();
        exit 1;
    }
}

sub kill_server {
    local ( $param_1 ) = @_;
    $result = $param_1->TerminateWaitKill ($server->ProcessStopWaitInterval());
    if ($result != 0) {
        print STDERR "ERROR: server returned $result\n";
        $status = 1;
    }
    $server->DeleteFile ($srv_ior);
}

sub run_client {
    local ( $param_1 ) = @_;
    $result = $param_1->Spawn();
    if ($result != 0) {
        print STDERR "ERROR: client returned $result\n";
        $status = 1;
    }
}

sub test {
    local($param_0, $param_1, $param_2, $param_3) = @_;
    print STDERR $param_0;

    $result = $param_1->Spawn();

    if ($result != 0) {
        print STDERR "ERROR: client returned $result\n";
        $status = 1;
    }

    $result = $param_1->Spawn();

    if ($result != 0) {
        print STDERR "ERROR: client returned $result\n";
        $status = 1;
    }

    $result = $param_1->Spawn();

    if ($result != 0) {
        print STDERR "ERROR: client returned $result\n";
        $status = 1;
    }

    $param_1->WaitKill ($client1->ProcessStopWaitInterval() + 20);
    $param_2->WaitKill ($client2->ProcessStopWaitInterval() + 15);
    $param_3->WaitKill ($client3->ProcessStopWaitInterval() + 15);
    sleep 2;

    if ($param_0 =~ /Test1/) {
        evaluate_output();
    }
    $client1->DeleteFile ($client1_output);
    $client2->DeleteFile ($client2_output);
    $client3->DeleteFile ($client3_output);
}

sub test5 {
    print STDERR "\n==== Testing exceptions\n";
    print STDERR "\n==== Test5 - Testing ClientScheduler ".
                 "exception for invalid activity name\n".
                 "Should recieve an RTCosScheduling::UnknownName exeption\n";
    run_client($C4);
    $C4->WaitKill($client->ProcessStopWaitInterval());
}

sub test6 {
    print STDERR "\n==== Test6 - Testing client exception ".
                 "when invalid config file specified\n".
                 "Program should abort because no valid file was given\n";
    run_client($C5);
    $C5->WaitKill ($client->ProcessStopWaitInterval());
}

sub test7 {
    print STDERR "\n==== Test7 - Testing server exception ".
                 "when invalid Object Name specified\n";
    print STDERR "==== (Object name not in config file)\n".
                 "Should receive an RTCosScheduling::UnknownName exception\n";
    $S3->Spawn();
    sleep 1;
    kill_server($S3);
}

sub test8 {
    print STDERR "\n==== Test8 - Testing server exception ".
                 "when invalid config file specified\n".
                 "Server Should abort because an invalid config ".
                 "filename was given\n";
    $S2->Spawn();
    sleep 1;
    kill_server($S2);
}

sub evaluate_output {
    # this looks at the three output files to see if the
    # clients ran in the correct order
    open(INFOC1, $client1_output);
    open(INFOC2, $client2_output);
    open(INFOC3, $client3_output);

    @arrayC1=<INFOC1>;
    close (INFOC1);
    @arrayC2=<INFOC2>;
    close (INFOC2);
    @arrayC3=<INFOC3>;
    close (INFOC3);

    $is_valid = 1;

    # check the client execution to make sure
    #it runs in the correct order
    ($BAtimeC1,$client,$activity)=split(/\t/,$arrayC1[0]);
    ($BAtimeC2,$client,$activity)=split(/\t/,$arrayC2[0]);
    ($BAtimeC3,$client,$activity)=split(/\t/,$arrayC3[0]);

    ($CMtimeC1,$client,$activity)=split(/\t/,$arrayC1[1]);
    ($CMtimeC2,$client,$activity)=split(/\t/,$arrayC2[1]);
    ($CMtimeC3,$client,$activity)=split(/\t/,$arrayC3[1]);

    ($BRtimeC1,$client,$activity)=split(/\t/,$arrayC1[2]);
    ($BRtimeC2,$client,$activity)=split(/\t/,$arrayC2[2]);
    ($BRtimeC3,$client,$activity)=split(/\t/,$arrayC3[2]);

    ($FRtimeC1,$client,$activity)=split(/\t/,$arrayC1[3]);
    ($FRtimeC2,$client,$activity)=split(/\t/,$arrayC2[3]);
    ($FRtimeC3,$client,$activity)=split(/\t/,$arrayC3[3]);

    ($DMtimeC1,$client,$activity)=split(/\t/,$arrayC1[4]);
    ($DMtimeC2,$client,$activity)=split(/\t/,$arrayC2[4]);
    ($DMtimeC3,$client,$activity)=split(/\t/,$arrayC3[4]);

    ($DTtimeC1,$client,$activity)=split(/\t/,$arrayC1[5]);
    ($DTtimeC2,$client,$activity)=split(/\t/,$arrayC2[5]);
    ($DTtimeC3,$client,$activity)=split(/\t/,$arrayC3[5]);

    if ($BAtimeC1 gt $BAtimeC2 || $BAtimeC1 gt $BAtimeC3) {
        print STDERR "Scheduler Test Failed - activities did not begin in correct order\n";
        $is_valid = 0;
    }
    if ($CMtimeC1 gt $CMtimeC2 || $CMtimeC1 gt $CMtimeC3) {
        print STDERR "Scheduler Test Failed - remote method calls not ".
                     "made in correct order\n";
        $is_valid = 0;
    }
    if ($FRtimeC1 gt $FRtimeC3 || $FRtimeC3 gt $FRtimeC2) {
        print STDERR "Scheduler Test Failed - Remote Method calls did ".
                     "not finish in correct order\n";
        $is_valid = 0;
    }
    if ($DTtimeC3 gt $DTtimeC2  || $DTtimeC2 gt $DTtimeC1) {
        print STDERR "Scheduler Test Failed - Tests did not finish in ".
                     "correct order\n";
        $is_valid = 0;
    }
    if ($BRtimeC1 gt $BAtimeC2) {
        print STDERR "Scheduler Test Failed - Client1 remote method call ".
                     "finished before Client2 began local activity\n";
        $is_valid = 0;
    }
    if ($FRtimeC1 gt $BRtimeC3 || $FRtimeC1 gt $BRtimeC2) {
        print STDERR "Scheduler Test Failed - Client1 finished before ".
                     "Clients2 or 3 made method calls\n";
        $is_valid = 0;
    }
    if ($BRtimeC3 gt $BRtimeC2 || $FRtimeC3 gt $FRtimeC2) {
        print STDERR "Scheduler Test Failed - Client 3 remote method call ".
                     "did not begin before Client2\n";
        $is_valid = 0;
    }

    #next one condition disables WARNING only. No functionality, no logic.
    if ($DMtimeC1 || $DMtimeC2 || $DMtimeC3) {
    }

    if ($is_valid == 1) {
        print STDERR "The scheduling service worked as expected\n";
    }
    else {
        print STDERR "The scheduling service did not work as expected\n";
    }

}

print STDERR "\n==== Running RTCORBA 1.0 Scheduling Service test\n";
print STDERR "==== Note that the first column is the time, it will be different for you\n\n";
print STDERR "TIME\t\t\tOBJECT\tLOCATION\tACTIVITY\n";

spawn_server($S11);
test("\n==== Test1 - YES client side scheduling, YES server side scheduling\n", $C11, $C21, $C31);
test("\n==== Test2 - NO client side scheduling, YES server side scheduling\n", $C10, $C20, $C30);
kill_server($S11);

spawn_server($S10);
test("\n==== Test3 - YES client side scheduling, NO server side scheduling\n", $C11, $C21, $C31);
test("\n==== Test4 - NO client side scheduling, NO server side scheduling\n",  $C10, $C20, $C30);
kill_server ($S10);

spawn_server ($S11);

test5 ();
test6 ();

kill_server ($S11);

test7 ();
test8 ();

exit $status;
