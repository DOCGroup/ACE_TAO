eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$debug_level = '0';
foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '1';
    }
}

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
$server->DeleteFile($iorbase);

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$SV = $server->CreateProcess ("client", "-ORBdebuglevel $debug_level -k $server_iorfile");

$test = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($test != 0) {
    print STDERR "ERROR: test returned $test\n";
    exit 1;
}

$server->DeleteFile($iorbase);

exit 0;
