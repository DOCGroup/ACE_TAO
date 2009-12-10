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
$T = $test->CreateProcess ("registration", "-ORBdebuglevel $debug_level");
$test_status = $T->SpawnWaitKill ($test->ProcessStartWaitInterval() + 45);

if ($test_status != 0) {
    print STDERR "ERROR: test returned $test_status\n";
    exit 1;
}

exit $status;
