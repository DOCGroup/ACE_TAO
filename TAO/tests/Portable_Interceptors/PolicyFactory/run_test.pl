eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-
#
# $Id$

use lib  "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $status = 0;

my $process = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$SV = $process->CreateProcess ("server");

print STDERR "\n\n==== Running PolicyFactory test\n";

$SV->Spawn ();

$process_status = $SV->WaitKill ($process->ProcessStopWaitInterval() + 30); # extra timeout

if ($process_status != 0) {
    print STDERR "ERROR: server returned $process_status\n";
    $status = 1;
}

exit $status;
