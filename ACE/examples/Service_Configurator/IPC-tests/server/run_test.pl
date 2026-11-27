eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use strict;
use warnings;
use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $target = PerlACE::TestTarget::create_target (1)
    || die "Create target 1 failed\n";
$target->AddLibPath ('.');
my $test = $target->CreateProcess ('ipc_service_test');
exit $test->SpawnWaitKill ($target->ProcessStartWaitInterval ());
