eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

print STDERR "\n********** RTCORBA Destroy_Thread_Pool Unit Test **********\n\n";

$SV = $server->CreateProcess ("Destroy_Thread_Pool");

$server_status = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: test returned $server_status\n";
    exit 1;
}

exit 0;
