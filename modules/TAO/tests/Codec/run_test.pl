eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-
# $Id$

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $target = PerlACE::TestTarget::create_target(1) || die "Create target 1 failed\n";

print STDERR "\n\n==== Running Codec test\n";

$T = $target->CreateProcess ("client");

$test = $T->SpawnWaitKill ($target->ProcessStartWaitInterval());

$target->GetStderrLog();

if ($test != 0) {
    print STDERR "ERROR: Codec test returned $test\n";
    exit 1;
}

exit 0;
