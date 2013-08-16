eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $target1 = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $check = $target1->CreateProcess ("check");
my $result = $check->SpawnWaitKill ($target1->ProcessStartWaitInterval());
if ($result != 0) {
    print STDERR "ERROR: check returned $result\n";
    exit 1;
}

exit 0;
