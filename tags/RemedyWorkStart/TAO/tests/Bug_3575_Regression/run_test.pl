eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $client = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$CL = $client->CreateProcess ("client", "-ORBInitRef MyObjectId=file://myobj.ior");

$test = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($test != 0) {
    print STDERR "ERROR: test returned $test\n";
    exit 1;
}

exit 0;
