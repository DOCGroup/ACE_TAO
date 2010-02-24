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

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $iorbase = "test.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
$server->DeleteFile($iorbase);

$SV = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level");

print STDERR "\n\n==== Running bug 3486 regression test\n";

$server_status = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: Bug_3486_Regression returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);

exit $status;
