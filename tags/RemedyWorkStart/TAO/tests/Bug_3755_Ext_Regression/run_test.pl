eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';
$rt = 0;
$rtsched = 0;
$endpoint = 0;
$diffserv = 0;
$bidir = 0;
$messaging = 0;
$csd = 0;
$ziop = 0;

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    } elsif ($i eq '-rt') {
        $rt = 1;
    } elsif ($i eq '-rtsched') {
        $rtsched = 1;
    } elsif ($i eq '-endpoint') {
        $endpoint = 1;
    } elsif ($i eq '-diffserv') {
        $diffserv = 1;
    } elsif ($i eq '-bidir') {
        $bidir = 1;
    } elsif ($i eq '-messaging') {
        $messaging = 1;
    } elsif ($i eq '-csd') {
        $csd = 1;
    } elsif ($i eq '-ziop') {
        $ziop = 1;
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

sub run_one_test {
    my $name = shift;
    my $svc1 = shift;
    my $svc2 = shift;

    print "\nRunning $name test\n";

    my $server_svc_conf1 = $server->LocalFile ($svc1);
    my $server_svc_conf2 = $server->LocalFile ($svc2);
    my $dopt = "-ORBdebuglevel $debug_level";

    $SV = $server->CreateProcess ("server",
        "-a \"AAA $dopt -ORBSvcConf $server_svc_conf1\" " .
        "-b \"BBB $dopt -ORBGestalt LOCAL -ORBSvcConf $server_svc_conf2\"");
    $server_status = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

    if ($server_status != 0) {
        print STDERR "ERROR: server returned $server_status\n";
        return 1;
    }

    print "$name test succeeded\n";
    return 0;
}

# Common tests.
my @svcs = (
    ["PI_Server", "pi_server_svc.conf", "pi_server_svc.conf"],
);

if ($rt) {
    @svcs = (
        ["RTCORBA", "rt_svc.conf", "rt_svc.conf"],
    );
} elsif ($rtsched) {
    @svcs = (
        ["RTScheduler", "rt_scheduler_svc.conf", "rt_scheduler_svc.conf"],
    );
} elsif ($endpoint) {
    @svcs = (
        ["EndpointPolicy", "ep_svc.conf", "ep_svc.conf"],
    );
} elsif ($diffserv) {
    @svcs = (
        ["DiffServPolicy", "dp_svc.conf", "dp_svc.conf"]
    );
} elsif ($bidir) {
    @svcs = (
        ["BiDir_GIOP", "bidir_svc.conf", "bidir_svc.conf"],
    );
} elsif ($messaging) {
    @svcs = (
        ["Messaging", "messaging_svc.conf", "messaging_svc.conf"],
    );
} elsif ($csd) {
    @svcs = (
        ["CSD_Framework", "csd_svc.conf", "csd_svc.conf"],
    );
} elsif ($ziop) {
    @svcs = (
        ["ZIOP", "ziop_svc.conf", "ziop_svc.conf"],
    );
}

foreach my $svc_pair (@svcs) {
    $status += run_one_test (@$svc_pair);
}

exit $status;
