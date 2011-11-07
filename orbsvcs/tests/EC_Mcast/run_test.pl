eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

$event_count = '100';
$event_period = '50000';
$federation = 'Set02';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $config_base = "sample.cfg";
my $server_config = $server->LocalFile ($config_base);
my $client_config = $client->LocalFile ($config_base);

my $server_svc = $server->LocalFile ("svc" . "$PerlACE::svcconf_ext");
my $client_svc = $client->LocalFile ("svc" . "$PerlACE::svcconf_ext");

$SV = $server->CreateProcess ("EC_Mcast",
                              "-ORBdebuglevel $debug_level " .
                              "-ORBSvcConf $server_svc " .
                              "-c $server_config " .
                              "-n $event_count " .
                              "-t $event_period " .
                              "-f $federation");

$CL = $client->CreateProcess ("EC_Mcast",
                              "-ORBdebuglevel $debug_level " .
                              "-ORBSvcConf $client_svc " .
                              "-c $client_config " .
                              "-n $event_count " .
                              "-t $event_period " .
                              "-f $federation");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

$client_status = $CL->Spawn ();

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    exit 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval() + 45);
$client_status = $CL->WaitKill ($client->ProcessStopWaitInterval() + 45);

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    exit 1;
}

exit $status;
