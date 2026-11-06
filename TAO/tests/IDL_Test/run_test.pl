eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $process = $server->CreateProcess ("main");
my $status = $process->SpawnWaitKill ($server->ProcessStartWaitInterval());
if ($status != 0) {
    print STDERR "ERROR: test returned $status\n";
}

exit $status;
