eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $target = PerlACE::TestTarget::create_target (1)
  || die "Create target failed\n";

my $test = $target->CreateProcess ("exclusive_tms_completion");
my $status = $test->SpawnWaitKill ($target->ProcessStartWaitInterval ());

if ($status != 0) {
    print STDERR "ERROR: exclusive_tms_completion returned $status\n";
    exit 1;
}

exit 0;
