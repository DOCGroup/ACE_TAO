eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$ciao_root = "$ENV{CIAO_ROOT}";

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $ENV{"DANCE_TRACE_ENABLE"} = 1;
        $ENV{"CIAO_TRACE_ENABLE"} = 1;
    }
}

my $target = PerlACE::TestTarget::create_target ($PerlACE::TestConfig);

$TG = $target->CreateProcess ("client", "-s $ciao_root/bin/ciao_componentserver -d 120");
$server_status = $TG->SpawnWaitKill ($target->ProcessStartWaitInterval ());

if ($server_status != 0) {
    print STDERR "ERROR: process returned $server_status\n";
    exit 1;
}

$target->GetStderrLog();

exit $status;
