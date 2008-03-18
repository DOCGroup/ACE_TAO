eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use PerlACE::TestTarget;

$target = PerlACE::TestTarget::create_target($PerlACE::TestConfig) || die "Create target failed\n";

$status = 0;
$debug_level = '0';
$test_port = 12345;
$management_port = 12346;
$client_conf = "lf_flush.conf";
$flush_strategy = "LF";

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
    elsif ($i eq '-blocking') {
	$client_conf = "block_flush.conf";
	$flush_strategy = "BLOCKING";
    }
    elsif ($i eq '-reactive') {
        $client_conf = "reactive_flush.conf";
        $flush_strategy = "REACTIVE";
    }
    else {
	print STDERR "ERROR: Unknown option: $i\n";
	exit 1;
    }
}

my $test_opts = "-s \'-ORBEndpoint iiop://localhost:$test_port -ORBEndpoint " .
    "iiop://localhost:$management_port -ORBDebuglevel $debug_level -ORBCollocation " .
    "no -ORBGestalt LOCAL\' -c \'-k corbaloc::localhost:$test_port/Test -b " .
    "corbaloc::localhost:$management_port/Management -ORBDebuglevel $debug_level " .
    "-ORBCollocation no -1 -ORBSvcConf $client_conf -f $flush_strategy " .
    "-ORBGestalt LOCAL\'";

if (PerlACE::is_vxworks_test()) {
    $TV = new PerlACE::ProcessVX ("oneway_test", "$test_opts");
}
else {
    $TV = new PerlACE::Process ("oneway_test", "$test_opts");
}

print $TV->CommandLine()."\n";
$test = $TV->Spawn ();

if ($test != 0) {
    print STDERR "ERROR: test returned $test\n";
    exit 1;
}

$test = $TV->WaitKill (10);

if ($test != 0) {
    print STDERR "ERROR 2: test returned $test\n";
    $status = 1;
}

exit $status;
