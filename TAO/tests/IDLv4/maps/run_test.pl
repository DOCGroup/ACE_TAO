eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use strict;
use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $tao_idl = "$ENV{ACE_ROOT}/bin/tao_idl";
if (exists $ENV{HOST_ROOT}) {
    $tao_idl = "$ENV{HOST_ROOT}/bin/tao_idl";
}

my $target = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $proc = $target->CreateProcess ("maps");

my $test = $proc->SpawnWaitKill ($target->ProcessStartWaitInterval ());

if ($test != 0) {
    print STDERR "ERROR: test returned $test\n";
    exit 1;
}

exit 0;
