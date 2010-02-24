eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$continuous = ($^O eq 'hpux');

print STDERR "\n********** RTCORBA Collocation Unit Test **********\n\n";

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$T = $server->CreateProcess ("Collocation");

if ($continuous) {
    $T->Arguments("-ORBSvcConf continuous$PerlACE::svcconf_ext");
}

$test = $T->SpawnWaitKill($server->ProcessStartWaitInterval ());
if ($test == 2) {
    # Mark as no longer running to avoid errors on exit.
    $T->{RUNNING} = 0;
} else {
    if ($test != 0) {
        print STDERR "ERROR: test returned $test\n";
        exit 1;
    }
}

exit 0;
