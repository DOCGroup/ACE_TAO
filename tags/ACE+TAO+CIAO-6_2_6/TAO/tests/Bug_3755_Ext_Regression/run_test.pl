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

    # copy the configuation files
    if ($server->PutFile ($svc1) == -1) {
        print STDERR "ERROR: cannot set file <$server_svc_conf1>\n";
        return 1;
    }
    if ($server->PutFile ($svc2) == -1) {
        print STDERR "ERROR: cannot set file <$server_svc_conf2>\n";
        return 1;
    }

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
my @svcs = ();

# add common runtime loaded library dependencies
$server->AddRuntimeLibrary('TAO_PortableServer');
$server->AddRuntimeLibrary('TAO_PI');
$server->AddRuntimeLibrary('TAO_AnyTypeCode');
$server->AddRuntimeLibrary('TAO_CodecFactory');

if ($rt) {
    @svcs = (
        ["RTCORBA", "rt_svc.conf", "rt_svc.conf"],
    );
    $server->AddRuntimeLibrary('TAO_RTCORBA');
} elsif ($rtsched) {
    @svcs = (
        ["RTScheduler", "rt_scheduler_svc.conf", "rt_scheduler_svc.conf"],
    );
    $server->AddRuntimeLibrary('TAO_RTCORBA');
    $server->AddRuntimeLibrary('TAO_RTScheduler');
    $server->AddRuntimeLibrary('TAO_Messaging');
    $server->AddRuntimeLibrary('TAO_Valuetype');
    $server->AddRuntimeLibrary('TAO_PI_Server');
} elsif ($endpoint) {
    @svcs = (
        ["EndpointPolicy", "ep_svc.conf", "ep_svc.conf"],
    );
    $server->AddRuntimeLibrary('TAO_EndpointPolicy');
} elsif ($diffserv) {
    @svcs = (
        ["DiffServPolicy", "dp_svc.conf", "dp_svc.conf"]
    );
    $server->AddRuntimeLibrary('TAO_DiffServPolicy');
} elsif ($bidir) {
    @svcs = (
        ["BiDir_GIOP", "bidir_svc.conf", "bidir_svc.conf"],
    );
    $server->AddRuntimeLibrary('TAO_BiDirGIOP');
} elsif ($messaging) {
    @svcs = (
        ["Messaging", "messaging_svc.conf", "messaging_svc.conf"],
    );
    $server->AddRuntimeLibrary('TAO_Messaging');
    $server->AddRuntimeLibrary('TAO_Valuetype');
} elsif ($csd) {
    @svcs = (
        ["CSD_Framework", "csd_svc.conf", "csd_svc.conf"],
    );
    $server->AddRuntimeLibrary('TAO_CSD_Framework');
    $server->AddRuntimeLibrary('TAO_CSD_ThreadPool');
} elsif ($ziop) {
    @svcs = (
        ["ZIOP", "ziop_svc.conf", "ziop_svc.conf"],
    );
    $server->AddRuntimeLibrary('TAO_ZIOP');
    $server->AddRuntimeLibrary('TAO_Compression');
} else {
    @svcs = (
        ["PI_Server", "pi_server_svc.conf", "pi_server_svc.conf"],
    );
    $server->AddRuntimeLibrary('TAO_PI_Server');
}

foreach my $svc_pair (@svcs) {
    $status += run_one_test (@$svc_pair);
}

exit $status;
