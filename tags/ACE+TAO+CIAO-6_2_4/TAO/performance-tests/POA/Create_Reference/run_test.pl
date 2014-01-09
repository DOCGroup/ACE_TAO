eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$iterations = 10000;

print STDERR "================ Reference Creation Test\n";

print STDERR "\nTesting with hints (default configuration)\n";

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$SV = $server->CreateProcess ("create_reference", " -i $iterations");

$status = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval() + 100);

if ($status != 0) {
    print STDERR "ERROR: create_reference returned $status\n";
    exit $status;
}

print STDERR "\nTesting without hints (optimized configuration)\n";

$no_active_hint_directive = "-ORBsvcconfdirective \"static Server_Strategy_Factory '-ORBActiveHintInIds 0'\"";

$SV->Arguments ("-i $iterations $no_active_hint_directive");

$status = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval() + 100);

if ($status != 0) {
    print STDERR "ERROR: create_reference returned $status\n";
    exit $status;
}

exit $status;
