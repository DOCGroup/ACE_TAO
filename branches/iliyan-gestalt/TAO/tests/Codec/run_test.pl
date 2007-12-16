eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-
# $Id$

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use PerlACE::TestTarget;

my $target = PerlACE::TestTarget::create_target ($PerlACE::TestConfig);

print STDERR "\n\n==== Running Codec test\n";

if (PerlACE::is_vxworks_test()) {
    $T = new PerlACE::ProcessVX ("client");
}
else {
    $T = $target->CreateProcess ("client");
}

$test = $T->SpawnWaitKill ($PerlACE::wait_interval_for_process_creation);

$target->GetStderrLog();

if ($test != 0) {
    print STDERR "ERROR: Codec test returned $test\n";
    exit 1;
}

exit 0;
