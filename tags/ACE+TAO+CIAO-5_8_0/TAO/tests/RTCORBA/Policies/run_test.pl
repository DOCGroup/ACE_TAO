eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $policies = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$SV = $policies->CreateProcess ("Policies");
$status = $SV->SpawnWaitKill ($policies->ProcessStartWaitInterval ());

if ($status != 0) {
    print STDERR "ERROR: test returns $status\n";
    exit 1;
}

exit 0;
