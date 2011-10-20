eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$target = PerlACE::TestTarget::create_target(1) || die "Create target failed\n";

$status = 0;
$debug_level = '0';
$test_port = 12345;
$management_port = 12346;
$conf_file = "lf_flush.conf";
$flush_strategy = "LF";

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
    elsif ($i eq '-blocking') {
        $conf_file = "block_flush.conf";
        $flush_strategy = "BLOCKING";
    }
    elsif ($i eq '-reactive') {
        $conf_file = "reactive_flush.conf";
        $flush_strategy = "REACTIVE";
    }
    else {
        print STDERR "ERROR: Unknown option: $i\n";
        exit 1;
    }
}

$client_conf = $target->LocalFile ($conf_file);

if ($target->PutFile ($conf_file) == -1) {
    print STDERR "ERROR: cannot set file <$client_conf>\n";
    exit 1;
}

my $test_opts = "-s \'-ORBEndpoint iiop://localhost:$test_port -ORBEndpoint " .
    "iiop://localhost:$management_port -ORBDebuglevel $debug_level -ORBCollocation " .
    "no -ORBGestalt LOCAL\' -c \'-k corbaloc::localhost:$test_port/Test -b " .
    "corbaloc::localhost:$management_port/Management -ORBDebuglevel $debug_level " .
    "-ORBCollocation no -1 -ORBSvcConf $client_conf -f $flush_strategy " .
    "-ORBGestalt LOCAL\'";

$TV = $target->CreateProcess ("oneway_test", "$test_opts");

$test = $TV->Spawn ();

if ($test != 0) {
    print STDERR "ERROR: test returned $test\n";
    exit 1;
}

$test = $TV->WaitKill ($target->ProcessStartWaitInterval() +
                       $target->ProcessStopWaitInterval());

if ($test != 0) {
    print STDERR "ERROR 2: test returned $test\n";
    $status = 1;
}

exit $status;
