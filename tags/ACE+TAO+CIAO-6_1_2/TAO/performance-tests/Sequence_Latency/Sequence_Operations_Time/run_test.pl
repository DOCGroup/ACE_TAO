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

print STDERR "================ Sequence Operations Time Test\n";

for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") {
        print "Run_Test Perl script for Performance Test\n\n";
        print "run_test \n";
        print "\n";
        exit 0;
    }
}

my $client = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$CL = $client->CreateProcess ("test", "-ORBdebuglevel $debug_level");

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 405);

if ($test_status != 0) {
    print STDERR "ERROR: test returned $client_status\n";
    $status = 1;
}

exit $status;
