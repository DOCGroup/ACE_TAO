eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

$status = 0;

$sample_cfg = PerlACE::LocalFile ("sample.cfg");
$svc_conf = PerlACE::LocalFile ("svc.conf");

# Run two copies of the same test...
$T1 = new PerlACE::Process ("EC_Mcast",
                            "-c $sample_cfg -ORBSvcConf $svc_conf "
                            . "-n 100 -t 50000 -f Set02");
$T2 = new PerlACE::Process ("EC_Mcast",
                            "-c $sample_cfg -ORBSvcConf $svc_conf "
                            . "-n 100 -t 50000 -f Set02");

$T1->Spawn ();
$T2->Spawn ();

$test1 = $T1->WaitKill (60);

if ($test1 != 0) {
    print STDERR "ERROR: test 1 returned $test1\n";
    $status = 1;
}

$test2 = $T2->WaitKill (60);

if ($test2 != 0) {
    print STDERR "ERROR: test 2 returned $test2\n";
    $status = 1;
}

exit $status;
