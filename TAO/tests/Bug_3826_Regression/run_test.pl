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

my $svc_conf = "orb_svc.conf";
my $server_svc_conf = $server->LocalFile ($svc_conf);

print STDOUT "Testing with -ORBSvcConf\n";

$SV = $server->CreateProcess ("server",
    "-a \"AAA -ORBdebuglevel $debug_level -ORBSvcConf $server_svc_conf -ORBGestalt LOCAL\" " .
    "-b \"BBB\"");

$server_status = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

print STDOUT "Testing with -ORBSvcConfDirective\n";

$SV = $server->CreateProcess ("server",
    "-a \"AAA -ORBdebuglevel $debug_level -ORBGestalt LOCAL " .
        "-ORBSvcConfDirective \\\"static Resource_Factory \\\\\\\"-ORBConnectionCacheMax 1\\\\\\\"\\\"\" " .
    "-b \"BBB\"");

$server_status = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

exit $status;
