eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::TestTarget;

$status = 0;
$is_sciop = 0;

for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "--sciop") {
        $is_sciop = 1;
    }
}

my $test = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$port = $test->RandomPort ();
$multicast = '225.1.1.8';

my $iorbase = "server.ior";
my $test_iorfile = $test->LocalFile ($iorbase);
$test->DeleteFile($iorbase);

my $log = "test.log";
my $log_file = $test->LocalFile ($log);
$test->DeleteFile ($log);

sub run_test {
    $test_name = shift;
    $test_conf = shift;
    $test_c_params = shift;
    $test_s_params = shift;

    $result = 0;

    $test->DeleteFile($iorbase);
    $test->DeleteFile($log);

    my($svc_conf);
    if ($test_conf ne ''){
        $tmp_test_conf = $test->LocalFile ($test_conf);
        $svc_conf = "-ORBSvcConf $tmp_test_conf";
    }

    $SV = $test->CreateProcess ("server", " -o $test_iorfile $test_s_params  $svc_conf");
    $CL = $test->CreateProcess ("client", "-k file://$test_iorfile " .
                                  "$test_c_params $svc_conf " .
                                  "-ORBIPHopLimit 1 -ORBDebugLevel 1 -ORBLogFile $log_file");

    print STDOUT "Starting $test_name\n";
    print STDOUT "  server " . $SV->Arguments () . "\n";

    $server_status = $SV->Spawn ();

    if ($server_status != 0) {
        print STDERR "ERROR: $test_name failed - server returned $server_status\n";
        return 1;
    }

    if ($test->WaitForFileTimed ($iorbase,
                               $test->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$test_iorfile>\n";
        $SV->Kill (); $SV->TimedWait (1);
        exit 1;
    }

    print STDOUT "  client " . $CL->Arguments () . "\n";

    $client_status = $CL->SpawnWaitKill ($test->ProcessStartWaitInterval());

    if ($client_status != 0) {
        print STDERR "ERROR: $test_name failed - client returned $client_status\n";
        $status = 1;
    }

    $server_status = $SV->WaitKill ($test->ProcessStopWaitInterval());

    if ($server_status != 0) {
        print STDERR "ERROR: $test_name failed - server returned $server_status\n";
        $status = 1;
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

    $test->DeleteFile($iorbase);
    $test->DeleteFile($log);

    return $result;
}

$status = 0;

%tests = (
    "IIOP test"  => [ "", "", "" ],
    "DIOP test"  => [ "diop_svc.conf", "", "-ORBListenEndpoints diop://" ],
    "UIPMC test" => [ "miop_svc.conf", "-ORBIPMulticastLoop 1",
                      "-u corbaloc:miop:1.0\@1.0-cdmwftdomain-1/$multicast:$port" ]
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
