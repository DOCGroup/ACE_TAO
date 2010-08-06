eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::Run_Test;

$is_sciop = 0;

for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "--sciop") {
        $is_sciop = 1;
    }
}

sub run_test {
    $test_name = shift;
    $test_conf = shift;
    $test_c_params = shift;
    $test_s_params = shift;

    $result = 0;
    $plain_log_file = "test.log";
    $log_file = PerlACE::LocalFile ($plain_log_file);
    $plain_ior_file = "server.ior";
    $ior_file = PerlACE::LocalFile ($plain_ior_file);
    unlink $log_file, $ior_file;

    $svc_conf = '';
    if ($test_conf ne '') {
        if (PerlACE::is_vxworks_test()) {
            $svc_conf = "-ORBSvcConf $test_conf";
        } else {
            $tmp_test_conf = PerlACE::LocalFile ($test_conf);
            $svc_conf = "-ORBSvcConf $tmp_test_conf";
        }
    }

    $SV = new PerlACE::Process ("server", "-o $ior_file $test_s_params $svc_conf");
    if (PerlACE::is_vxworks_test()) {
        $CL = new PerlACE::ProcessVX (
            "client",
            "-k file://$plain_ior_file $test_c_params $svc_conf " .
            "-ORBIPHopLimit 1 -ORBDebugLevel 1 -ORBLogFile $plain_log_file");
    } else {
        $CL = new PerlACE::Process (
            "client",
            "-k file://$ior_file $test_c_params $svc_conf " .
            "-ORBIPHopLimit 1 -ORBDebugLevel 1 -ORBLogFile $log_file");
    }

    print STDOUT "Starting $test_name\n";
    print STDOUT "  server " . $SV->Arguments () . "\n";

    $server = $SV->Spawn ();

    if ($server != 0) {
        print STDERR "ERROR: $test_name failed - server returned $server\n";
        return 1;
    }

    if (PerlACE::waitforfile_timed ($ior_file,
                                    $PerlACE::wait_interval_for_process_creation) == -1) {
        print STDERR "ERROR: $test_name failed - cannot find file <$ior_file>\n";
        $SV->Kill ();
        return 1;
    }

    print STDOUT "  client " . $CL->Arguments () . "\n";

    $client = $CL->SpawnWaitKill (20);

    if ($client != 0) {
        print STDERR "ERROR: $test_name failed - client returned $client\n";
        $result = 1;
    }

    $server = $SV->WaitKill (10);

    if ($server != 0) {
        print STDERR "ERROR: $test_name failed - killing server returned $server\n";
        $result = 1;
    }

    print STDOUT "Client's log:\n";

    open (DATA, $log_file);
    while ($line = <DATA>) {
        # Process the line.
        chomp $line;

        if ($line =~ /hop limit/) {
            print STDERR "ERROR: $line\n";
            $result = 1;
        } elsif ($line =~ /multicast packets looping/) {
            print STDERR "ERROR: $line\n";
            $result = 1;
        } else {
            print STDOUT "$line\n";
        }
    }
    close (DATA);

    unlink $log_file, $ior_file;

    return $result;
}

$status = 0;

%tests = (
    "IIOP test"  => [ "", "", "-ORBConnectIPV6Only 1" ],
    "DIOP test"  => [ "diop_svc.conf", "", "-ORBConnectIPV6Only 1 -ORBListenEndpoints diop://" ],
    "UIPMC test" => [ "miop_svc.conf", "-ORBIPMulticastLoop 1",
                      "-u corbaloc:miop:1.0\@1.0-cdmwftdomain-1/[FF01:0:0:0:0:0:25:334]:23453 -ORBConnectIPV6Only 1" ]
);

if ($is_sciop == 1) {
    %tests = (
        "SCIOP test" => [ "sciop_svc.conf", "", "-ORBListenEndpoints sciop://" ]
    );
}

foreach $test (keys %tests) {
    $svc = $tests{$test}[0];
    $c_prms = $tests{$test}[1];
    $s_prms = $tests{$test}[2];

    $status = run_test ($test, $svc, $c_prms, $s_prms);
}

exit $status;
