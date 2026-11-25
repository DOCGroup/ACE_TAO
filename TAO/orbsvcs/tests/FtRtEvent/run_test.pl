eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $target = PerlACE::TestTarget::create_target (1) || die "Create target failed\n";
my $test = $target->CreateProcess ('uuid_test');
exit $test->SpawnWaitKill ($target->ProcessStartWaitInterval ());
