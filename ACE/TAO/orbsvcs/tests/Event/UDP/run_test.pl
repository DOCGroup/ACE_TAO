eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-
use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $sender   = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $receiver = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

$S = $sender->CreateProcess ("sender", "");
$R = $receiver->CreateProcess ("receiver", "");

sub run_with_arg{
    my $type = $_[0];
    my $arg = $_[1];

    $S->Arguments($arg);
    $R->Arguments($arg);

    print STDOUT "Starting receiver with $type\n";
    my $process_status = $R->Spawn ();
    if ($process_status != 0) {
        print STDERR "ERROR: receiver returned $process_status\n";
        return 1;
    }

    sleep 1;

    print STDOUT "Starting sender with $type\n";
    $process_status = $S->SpawnWaitKill ($sender->ProcessStartWaitInterval() + 135);
    if ($process_status != 0) {
        print STDERR "ERROR: sender returned $process_status\n";
        $status = 1;
    }

    $process_status = $R->WaitKill ($receiver->ProcessStopWaitInterval() + 135);

    if ($process_status != 0) {
        print STDERR "ERROR: receiver returned $receiver\n";
        $status = 1;
    }

    return 0;
}

run_with_arg ("plain text", "");
run_with_arg ("valuetype", "-v");

exit $status;
