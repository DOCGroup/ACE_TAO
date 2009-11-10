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
    			"-u corbaloc:miop:1.0\@1.0-cdmwftdomain-1/225.1.1.8:5555 " .
    			"-c corbaloc:miop:1.0\@1.0-cdmwftdomain-1/225.1.1.7:5555");


$test_status = $T->SpawnWaitKill (20);

if ($test_status != 0) {
    print STDERR "ERROR: uipmc_test returned $prog\n";
    $status = 1;
}

exit $status;