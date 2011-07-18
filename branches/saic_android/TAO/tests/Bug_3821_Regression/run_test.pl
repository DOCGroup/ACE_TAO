eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;

my $client = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$CL = $client->CreateProcess ("client");

print "Starting Test\n";
$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());
print "Ended Test\n";

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

exit $status;
