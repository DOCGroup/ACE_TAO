eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$debug = 0;

my $ior1file = "lm.ior";
my $ior2file = "group.ior";
my $ior3file = "direct_obj2.ior";

## The LoadManager needs to register signals with the ORB's reactor (on
## Windows only) and thus can not use the TP Reactor since it doesn't
## support that kind of thing.  So, we swith to the Select MT Reactor.
$lm_conf = $^O eq 'MSWin32' ? "windows$PerlACE::svcconf_ext"
                            : "non-windows$PerlACE::svcconf_ext";

$init_ref = "-ORBInitRef LoadManager=file://$ior1file";


@tests =
    (   {
         description => "Dead Member Detection",
         strategy  => "RoundRobin",
         svr1_args => "$init_ref -n 1 -o $ior2file",
         svr2_args => "$init_ref -n 2 -k file://$ior2file -j $ior3file",
         svr3_args => "$init_ref -n 3 -k file://$ior2file",
         client_args => "-k file://$ior2file -j file://$ior3file -s RoundRobin",
        }, {
         description => "Hang Member Detection",
         strategy  => "RoundRobin",
         svr1_args => "$init_ref -n 1 -o $ior2file",
         svr2_args => "$init_ref -n 2 -k file://$ior2file -j $ior3file -l 2",
         svr3_args => "$init_ref -n 3 -k file://$ior2file",
         client_args => "-k file://$ior2file -j file://$ior3file -s RoundRobin -l",
        }, {
         description => "Dead Member Detection",
         strategy  => "Random",
         svr1_args => "$init_ref -n 1 -o $ior2file",
         svr2_args => "$init_ref -n 2 -k file://$ior2file -j $ior3file",
         svr3_args => "$init_ref -n 3 -k file://$ior2file",
         client_args => "-k file://$ior2file -j file://$ior3file -s Random",
        }, {
         description => "Hang Member Detection",
         strategy  => "Random",
         svr1_args => "$init_ref -n 1 -o $ior2file",
         svr2_args => "$init_ref -n 2 -k file://$ior2file -j $ior3file -l 2",
         svr3_args => "$init_ref -n 3 -k file://$ior2file",
         client_args => "-k file://$ior2file -j file://$ior3file -s Random -l",
        },
    );


$status = 0;
for $test (@tests) {
    my $server1 = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
    my $server2 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
    my $server3 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
    my $server4 = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
    my $client = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";

    #Files which used by server1
    my $server1_ior1file = $server1->LocalFile ($ior1file);
    $server1->DeleteFile($ior1file);
    #Files which used by server2
    my $server2_ior1file = $server2->LocalFile ($ior2file);
    $server2->DeleteFile($ior2file);
    #Files which used by server3
    my $server3_ior1file = $server3->LocalFile ($ior3file);
    $server2->DeleteFile($ior3file);

    $status = 0;
    #-ORBVerboseLogging 1 -ORBDebugLevel 10 -ORBLogFile lm.log
    $LM = $server1->CreateProcess ("../../../../LoadBalancer/tao_loadmanager",
                                "-ORBVerboseLogging 1 -ORBDebugLevel $debug -s $test->{strategy} -o $ior1file -i 3"
                                . " -ORBSvcConf $lm_conf");
    $SV1 = $server2->CreateProcess ("server", $test->{svr1_args});
    $SV2 = $server3->CreateProcess ("server", $test->{svr2_args});
    $SV3 = $server4->CreateProcess ("server", $test->{svr3_args});
    $CL = $client->CreateProcess ("client", $test->{client_args});

    print STDERR "\n\n======== $test->{description} with $test->{strategy} ================\n";
    print STDERR "\n";

    print STDERR "== Start Load Banlancer.\n";
    print STDERR $LM->CommandLine () . "\n";

    $LM->Spawn ();

    if ($server1->WaitForFileTimed ($ior1file, $server1->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file LoadManager IOR: lm.ior\n";
        $LM->Kill (); $LM->TimedWait (1);
        exit 1;
    }

    if ($server1->GetFile ($ior1file) == -1) {
        print STDERR "ERROR: cannot retrieve file <$server1_ior1file>\n";
        $LM->Kill (); $LM->TimedWait (1);
        exit 1;
    }

    if ($server2->PutFile ($ior2file) == -1) {
        print STDERR "ERROR: cannot set file <$server2_ior2file>\n";
        $LM->Kill (); $LM->TimedWait (1);
        exit 1;
    }

    print STDERR "== Start 3 servers and add into a Object Group.\n";
    print STDERR $SV1->CommandLine () . "\n";
    $SV1->Spawn ();

    if ($server2->WaitForFileTimed ($ior2file, $server2->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find server file <$ior2file>\n";
        $LM->Kill ();
        $SV1->Kill (); $SV1->TimedWait (1);
        exit 1;
    }

    if ($server2->GetFile ($ior2file) == -1) {
        print STDERR "ERROR: cannot retrieve file <$server2_ior2file>\n";
        $LM->Kill ();
        $SV1->Kill (); $SV1->TimedWait (1);
        exit 1;
    }

    if ($server3->PutFile ($ior3file) == -1) {
        print STDERR "ERROR: cannot set file <$server3_ior1file>\n";
        $LM->Kill ();
        $SV1->Kill (); $SV1->TimedWait (1);
        exit 1;
    }

    sleep (2);

    print STDERR $SV2->CommandLine () . "\n";
    $SV2->Spawn ();

    if ($server3->WaitForFileTimed ($ior3file, $server3->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find server file <$ior3file>\n";
        $LM->Kill ();
        $SV1->Kill (); $SV1->TimedWait (1);
        $SV2->Kill (); $SV2->TimedWait (1);
        exit 1;
    }

    if ($server3->GetFile ($ior3file) == -1) {
        print STDERR "ERROR: cannot retrieve file <$server3_ior1file>\n";
        $LM->Kill ();
        $SV1->Kill (); $SV1->TimedWait (1);
        $SV2->Kill (); $SV2->TimedWait (1);
        exit 1;
    }

    sleep (2);

    print STDERR $SV3->CommandLine () . "\n";
    $SV3->Spawn ();

    sleep (2);

    print STDERR "== Start client.\n";

    print STDERR $CL->CommandLine () . "\n";
    $client = $CL->SpawnWaitKill($client->ProcessStartWaitInterval() + 85);

    if ($client != 0) {
        print STDERR "ERROR: client returned $client\n";
        $status = 1;
    }

    $server = $SV3->WaitKill ($server2->ProcessStopWaitInterval());

    if ($server != 0) {
        print STDERR "ERROR: server 3 returned $server\n";
        $status = 1;
    }

    $server = $SV2->WaitKill ($server2->ProcessStopWaitInterval());

    if ($server != 0) {
        print STDERR "ERROR: server 2 returned $server\n";
        $status = 1;
    }
    $server = $SV1->WaitKill ($server2->ProcessStopWaitInterval());

    if ($server != 0) {
        print STDERR "ERROR: server 1 returned $server\n";
        $status = 1;
    }

    $load_manager = $LM->TerminateWaitKill ($server2->ProcessStopWaitInterval());

    if ($load_manager != 0) {
        print STDERR "ERROR: LoadManager returned $load_manager\n";
        $status = 1;
    }

    $server1->DeleteFile($ior1file);
    $server2->DeleteFile($ior2file);
    $server3->DeleteFile($ior3file);

    if ($status == 0) {
      print STDERR "$description test passed.\n";
    }
    else {
      print STDERR "$description test failed.\n";
    }
}


exit $status;
