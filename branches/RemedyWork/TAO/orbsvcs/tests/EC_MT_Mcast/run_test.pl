eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $test1 = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $test2 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $svc_conf = "svc$PerlACE::svcconf_ext";
my $test1_svc_conf = $test1->LocalFile ($svc_conf);
my $test2_svc_conf = $test2->LocalFile ($svc_conf);

my $mcast_address = (int(rand(16)) + 224) . '.' . int(rand(256)) . '.' .
                     int(rand(256)) . '.' . int(rand(256)) . ':' .
                    (10001 + $test1->RandomPort());

# Run two copies of the same test...
$T1 = $test1->CreateProcess ("MCast", "-m $mcast_address -ORBSvcConf $test1_svc_conf");
$T2 = $test2->CreateProcess ("MCast", "-m $mcast_address -ORBSvcConf $test2_svc_conf");

$test_status = $T1->Spawn ();

if ($test_status != 0) {
    print STDERR "ERROR: could not spawn MCast 1, returned $test_status\n";
    exit 1;
}

$test_status = $T2->Spawn ();

if ($test_status != 0) {
    print STDERR "ERROR: could not spawn MCast 2, returned $test_status\n";
    $T1->Kill ();
    exit 1;
}

$test_status = $T1->WaitKill ($test1->ProcessStopWaitInterval() + 285);

if ($test_status != 0) {
    print STDERR "ERROR: test 1 returned $test_status\n";
    $status = 1;
}

$test_status = $T2->WaitKill ($test2->ProcessStopWaitInterval() + 285);

if ($test_status != 0) {
    print STDERR "ERROR: test 2 returned $test_status\n";
    $status = 1;
}

exit $status;
