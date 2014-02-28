eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";


$server_debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $server_debug_level = '10';
    }
}

$SV = $server->CreateProcess ("test", "-s -ORBdebuglevel $server_debug_level");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval()*2);

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

exit $status;
