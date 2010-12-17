eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

print STDERR "\n********** RTCORBA RTMutex Unit Test **********\n\n";

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$SV = $server->CreateProcess ("server");

$server_status = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval () + 45);

if ($server_status != 0) {
    print STDERR "ERROR: test returned $server_status\n";
    exit 1;
}

exit 0;
