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

my $svc_conf = "svc.conf";
my $orb_svc_conf = "orb_svc.conf";
my $server_svc_conf = $server->LocalFile ($svc_conf);
my $server_orb_svc_conf = $server->LocalFile ($orb_svc_conf);

# copy the configuation file
if ($server->PutFile ($orb_svc_conf) == -1) {
    print STDERR "ERROR: cannot set file <$server_orb_svc_conf>\n";
    return 1;
}
if ($server->PutFile ($svc_conf) == -1) {
    print STDERR "ERROR: cannot set file <$server_svc_conf>\n";
    return 1;
}

print STDOUT "Testing with -ORBSvcConf\n";

$SV = $server->CreateProcess ("server",
    "-a \"AAA -ORBdebuglevel $debug_level -ORBSvcConf $server_orb_svc_conf -ORBGestalt LOCAL\" " .
    "-b \"BBB\" -e 1");

$server_status = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

print STDOUT "Testing with default svc.conf\n";

$SV = $server->CreateProcess ("server",
    "-a \"AAA -ORBdebuglevel $debug_level -ORBGestalt LOCAL\" " .
    "-b \"BBB\" -e 2");

$server_status = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

print STDOUT "Testing with -ORBIgnoreDefaultSvcConfFile\n";

$SV = $server->CreateProcess ("server",
    "-a \"AAA -ORBdebuglevel $debug_level -ORBIgnoreDefaultSvcConfFile\" " .
    "-b \"BBB -ORBGestalt LOCAL\" -e 0"); # -e 0 means no expected value.

$server_status = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

print STDOUT "Testing with -ORBSvcConfDirective\n";

$directive = "static Resource_Factory \\\\\\\"-ORBConnectionCacheMax 3\\\\\\\"";
$SV = $server->CreateProcess ("server",
    "-a \"AAA -ORBdebuglevel $debug_level -ORBGestalt LOCAL " .
        "-ORBSvcConfDirective \\\"$directive\\\"\" " .
    "-b \"BBB\" -e 3");

$server_status = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

exit $status;
