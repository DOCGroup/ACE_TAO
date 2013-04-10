eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $program = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$PROG = $program->CreateProcess ("TimeConversion", "");
$program_status = $PROG->SpawnWaitKill ($program->ProcessStartWaitInterval());

if ($program_status != 0) {
    print STDERR "ERROR: TimeConversion returned $program_status\n";
    exit 1;
}

$exit_status = $PROG->WaitKill ($program->ProcessStopWaitInterval());

if ($exit_status != 0) {
    print STDERR "ERROR: TimeConversion returned $server_status\n";
    $status = 1;
}

exit $status;
