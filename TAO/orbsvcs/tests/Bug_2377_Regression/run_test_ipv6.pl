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

my $test = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$T = $test->CreateProcess("uipmc_test",
                          "-ORBdebuglevel $debug_level ".
                          "-u corbaloc:miop:1.0\@1.0-cdmwftdomain-1/[FF01:0:0:0:0:0:25:335]:3977 " .
                          "-c corbaloc:miop:1.0\@1.0-cdmwftdomain-1/[FF01:0:0:0:0:0:25:334]:3977");


$test_status = $T->SpawnWaitKill ($test->ProcessStartWaitInterval());

if ($test_status != 0) {
    print STDERR "ERROR: uipmc_test returned $test_status\n";
    $status = 1;
}

exit $status;
