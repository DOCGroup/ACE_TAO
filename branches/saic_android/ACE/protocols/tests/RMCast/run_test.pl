eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;

my $target1 = PerlACE::TestTarget::create_target (1);
my $target2 = PerlACE::TestTarget::create_target (1);

my $sender = $target1->CreateProcess("sender", "224.1.2.3:12345");
my $receiver = $target2->CreateProcess("receiver", "224.1.2.3:12345");

$receiver->Spawn ();

# Wait for receiver to start.
#
sleep ($target1->ProcessStartWaitInterval());

$sender->Spawn ();

$status = $receiver->WaitKill (40);

if ($status != 0) {
    print STDERR "ERROR: receiver returned $status\n";
    $status = 1;
}

$sender->Kill ();

$target1->GetStderrLog();
$target2->GetStderrLog();

exit $status;
