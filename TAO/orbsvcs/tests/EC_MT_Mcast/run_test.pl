eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

$svc_conf = PerlACE::LocalFile ("svc$PerlACE::svcconf_ext");

$mcast_address = (int(rand(16)) + 224) . '.' . int(rand(256)) . '.' .
                 int(rand(256)) . '.' . int(rand(256)) . ':' .
                 (10001 + PerlACE::uniqueid());

# Run two copies of the same test...
$T1 = new PerlACE::Process ("MCast",
                            "-m $mcast_address -ORBSvcConf $svc_conf");
$T2 = new PerlACE::Process ("MCast",
                            "-m $mcast_address -ORBSvcConf $svc_conf");

$sp1 = $T1->Spawn ();

if ($sp1 != 0) {
    print STDERR "ERROR: could not spawn MCast, returned $sp1\n";
    exit 1;
}

@sp2 = $T2->Spawn ();

if ($sp2 != 0) {
    print STDERR "ERROR: could not spawn MCast, returned $sp2\n";
    $T1->Kill ();
    exit 1;
}

$test1 = $T1->WaitKill (300);

if ($test1 != 0) {
    print STDERR "ERROR: test 1 returned $test1\n";
    $status = 1;
}

$test2 = $T2->WaitKill (30);

if ($test2 != 0) {
    print STDERR "ERROR: test 2 returned $test2\n";
    $status = 1;
}

exit $status;
