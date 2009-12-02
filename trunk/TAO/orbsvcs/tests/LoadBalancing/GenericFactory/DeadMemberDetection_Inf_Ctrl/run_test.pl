eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$debug = 0;

$group_iorfile = PerlACE::LocalFile ("group.ior");
$factory1_iorfile = PerlACE::LocalFile ("factory1.ior");
$factory2_iorfile = PerlACE::LocalFile ("factory2.ior");
$factory3_iorfile = PerlACE::LocalFile ("factory3.ior");
$direct_iorfile = PerlACE::LocalFile ("direct_obj2.ior");
unlink $group_iorfile;
unlink $factory1_iorfile;
unlink $factory2_iorfile;
unlink $factory3_iorfile;
unlink $direct_iorfile;

$lm_ior = "lm.ior";
unlink $lm_ior;

## The LoadManager needs to register signals with the ORB's reactor (on
## Windows only) and thus can not use the TP Reactor since it doesn't
## support that kind of thing.  So, we swith to the Select MT Reactor.
$lm_conf = $^O eq 'MSWin32' ? PerlACE::LocalFile ("windows$PerlACE::svcconf_ext") 
                            : PerlACE::LocalFile ("non-windows$PerlACE::svcconf_ext");

$init_ref = "-ORBInitRef LoadManager=file://lm.ior";


@tests =
  (
   {
    description => "Dead Member Detection",
    strategy  => "RoundRobin",
    svr_args => "$init_ref -o $group_iorfile -x file://$factory1_iorfile -y file://$factory2_iorfile -z file://$factory3_iorfile",
    svr1_args => "$init_ref -n 1 -o $factory1_iorfile",
    svr2_args => "$init_ref -n 2 -o $factory2_iorfile -j $direct_iorfile",
    svr3_args => "$init_ref -n 3 -o $factory3_iorfile",
    client_args => "-k file://$group_iorfile -j file://$direct_iorfile -s RoundRobin",
   },
   {
    description => "Hang Member Detection",
    strategy  => "RoundRobin",
    svr_args => "$init_ref -o $group_iorfile -x file://$factory1_iorfile -y file://$factory2_iorfile -z file://$factory3_iorfile",
    svr1_args => "$init_ref -n 1 -o $factory1_iorfile",
    svr2_args => "$init_ref -n 2 -o $factory2_iorfile -j $direct_iorfile -l 2",
    svr3_args => "$init_ref -n 3 -o $factory3_iorfile",
    client_args => "-k file://$group_iorfile -j file://$direct_iorfile -s RoundRobin -l",
   },
   {
    description => "Dead Member Detection",
    strategy  => "Random",
    svr_args => "$init_ref -o $group_iorfile -x file://$factory1_iorfile -y file://$factory2_iorfile -z file://$factory3_iorfile",
    svr1_args => "$init_ref -n 1 -o $factory1_iorfile",
    svr2_args => "$init_ref -n 2 -o $factory2_iorfile -j $direct_iorfile",
    svr3_args => "$init_ref -n 3 -o $factory3_iorfile",
    client_args => "-k file://$group_iorfile -j file://$direct_iorfile -s Random",
   },
   {
    description => "Hang Member Detection",
    strategy  => "Random",
    svr_args => "$init_ref -o $group_iorfile -x file://$factory1_iorfile -y file://$factory2_iorfile -z file://$factory3_iorfile",
    svr1_args => "$init_ref -n 1 -o $factory1_iorfile",
    svr2_args => "$init_ref -n 2 -o $factory2_iorfile -j $direct_iorfile -l 2",
    svr3_args => "$init_ref -n 3 -o $factory3_iorfile",
    client_args => "-k file://$group_iorfile -j file://$direct_iorfile -s Random -l",
   },
  );
  
  
$status = 0;
for $test (@tests)
  {
    $status = 0;
    #-ORBVerboseLogging 1 -ORBDebugLevel 10 -ORBLogFile lm.log 
    $LM = new PerlACE::Process ("../../../../LoadBalancer/LoadManager",
                                "-ORBVerboseLogging 1 -ORBDebugLevel $debug -s $test->{strategy} -o lm.ior -i 3" 
                                . " -ORBSvcConf $lm_conf");
    $SV = new PerlACE::Process ("server", $test->{svr_args});
    $SV1 = new PerlACE::Process ("factory_server", $test->{svr1_args});
    $SV2 = new PerlACE::Process ("factory_server", $test->{svr2_args});
    $SV3 = new PerlACE::Process ("factory_server", $test->{svr3_args});
    $CL = new PerlACE::Process ("client", $test->{client_args});

    print STDERR "\n\n======== $test->{description} with $test->{strategy} ================\n";
    print STDERR "\n";

    print STDERR "== Start Load Banlancer.\n";
    print STDERR $LM->CommandLine () . "\n";
    
    $LM->Spawn ();

    if (PerlACE::waitforfile_timed ("lm.ior", $PerlACE::wait_interval_for_process_creation) == -1) {
        print STDERR "ERROR: cannot find file LoadManager IOR: lm.ior\n";
        $LM->Kill (); $LM->TimedWait (1);
        exit 1;
    }

    print STDERR "== Start 3 servers and an app to add them to a Object Group.\n";
    print STDERR $SV1->CommandLine () . "\n";
    $SV1->Spawn ();

    if (PerlACE::waitforfile_timed ($factory1_iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
        print STDERR "ERROR: cannot find server file <$factory1_iorfile>\n";
        $LM->Kill ();
        $SV1->Kill (); $SV1->TimedWait (1);
        exit 1;
    }

    print STDERR $SV2->CommandLine () . "\n";
    $SV2->Spawn ();

    if (PerlACE::waitforfile_timed ($factory2_iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
        print STDERR "ERROR: cannot find server file <$factory2_iorfile>\n";
        $LM->Kill ();
        $SV1->Kill (); $SV1->TimedWait (1);
        $SV2->Kill (); $SV2->TimedWait (1);
        exit 1;
    }

    print STDERR $SV3->CommandLine () . "\n";
    $SV3->Spawn ();
    
    if (PerlACE::waitforfile_timed ($factory3_iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
        print STDERR "ERROR: cannot find server file <$factory3_iorfile>\n";
        $LM->Kill ();
        $SV1->Kill (); $SV1->TimedWait (1);
        $SV2->Kill (); $SV2->TimedWait (1);
        $SV3->Kill (); $SV3->TimedWait (1);
        exit 1;
    }

    print STDERR $SV->CommandLine () . "\n";
    $SV->Spawn ();
    
    if (PerlACE::waitforfile_timed ($group_iorfile, $PerlACE::wait_interval_for_process_creation * 4) == -1) {
        print STDERR "ERROR: cannot find server file <$group_iorfile>\n";
        $LM->Kill ();
        $SV1->Kill (); $SV1->TimedWait (1);
        $SV2->Kill (); $SV2->TimedWait (1);
        $SV3->Kill (); $SV3->TimedWait (1);
        $SV->Kill (); $SV->TimedWait (1);
        exit 1;
    }

    #Give enough time for group and member register with LB.
    sleep (5);

    print STDERR "== Start client.\n";

    print STDERR $CL->CommandLine () . "\n";

    $client = $CL->SpawnWaitKill(120);

    if ($client != 0) {
        print STDERR "ERROR: client returned $client\n";
        $status = 1;
    }

    $server = $SV3->WaitKill (30);

    if ($server != 0) {
        print STDERR "ERROR: server 3 returned $server\n";
        $status = 1;
    }

    $server = $SV2->WaitKill (30);

    if ($server != 0) {
        print STDERR "ERROR: server 2 returned $server\n";
        $status = 1;
    }
    $server = $SV1->WaitKill (30);

    if ($server != 0) {
        print STDERR "ERROR: server 1 returned $server\n";
        $status = 1;
    }
    $server = $SV->TerminateWaitKill (30);

    if ($server != 0) {
        print STDERR "ERROR: server returned $server\n";
        $status = 1;
    }
    #sleep 1000;
    $load_manager = $LM->TerminateWaitKill (60);

    if ($load_manager != 0) {
        print STDERR "ERROR: LoadManager returned $load_manager\n";
        $status = 1;
    }

    unlink $group_iorfile;
    unlink $factory1_iorfile;
    unlink $factory2_iorfile;
    unlink $factory3_iorfile;
    unlink $direct_iorfile;
    
    if ($status == 0)
    {
      print STDERR "$description test passed.\n";
    }
    else
    {
      print STDERR "$description test failed.\n";
    }
  }


exit $status;
