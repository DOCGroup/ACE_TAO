eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $test1 = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

my $persistent_file = "test.dat";

$test1->DeleteFile ($persistent_file);

$T1 = $test1->CreateProcess ("test", "-i 0");

my $test2 = PerlACE::TestTarget::create_target (2) || die "Create target 1 failed\n";

$T2 = $test2->CreateProcess ("test", "-i 1");

$T1->Spawn ();
$T2->SpawnWaitKill ($test2->ProcessStartWaitInterval());
$T1->WaitKill ($test1->ProcessStopWaitInterval());

$test1->DeleteFile ($persistent_file);
