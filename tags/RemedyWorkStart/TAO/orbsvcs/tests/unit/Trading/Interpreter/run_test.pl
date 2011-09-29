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

my($prog) = 'Interpreter';

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$SV = $server->CreateProcess ($prog, "-ORBdebuglevel $debug_level");

$status_server = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($status_server != 0) {
    print STDERR "ERROR: $prog returned $status_server\n";
    $status = 1;
}

exit $status;
