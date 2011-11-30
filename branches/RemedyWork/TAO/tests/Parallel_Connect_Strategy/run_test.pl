eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';
$quick = 0;

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    } elsif ($i eq '-quick') {
        $quick = 1;
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $TARGETHOSTNAME = $server->HostName ();
my $port = $server->RandomPort ();
my $port1 = $port + 1;
my $port2 = $port + 2;

my @bogus_eps = (
    "-ORBListenEndpoints iiop://$TARGETHOSTNAME:$port1/hostname_in_ior=126.0.0.123",
    "-ORBListenEndpoints iiop://$TARGETHOSTNAME:$port2/hostname_in_ior=126.0.0.124");
my $corbaloc = "corbaloc:".
    ":126.0.0.123:$port1,".
    ":$TARGETHOSTNAME:$port,".
    ":126.0.0.124:$port2/pcs_test";
my $valid_ep = "-ORBListenEndpoints iiop://$TARGETHOSTNAME:$port";

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

my $reactive_conf = "reactive$PerlACE::svcconf_ext";
my $blocked_conf = "blocked$PerlACE::svcconf_ext";
my $client_reactive_conf = $client->LocalFile ($reactive_conf);
my $client_blocked_conf = $client->LocalFile ($blocked_conf);

if ($client->PutFile ($reactive_conf) == -1) {
    print STDERR "ERROR: cannot set file <$client_reactive_conf>\n";
    exit 1;
}

if ($client->PutFile ($blocked_conf) == -1) {
    print STDERR "ERROR: cannot set file <$client_blocked_conf>\n";
    exit 1;
}

$SV = $server->CreateProcess ("server",
                              "-ORBdebuglevel $debug_level ".
                              "-ORBUseSharedProfile 1 ".
                              "-o $server_iorfile ".
                              "$bogus_eps[0] $valid_ep $bogus_eps[1]");
$CL = $client->CreateProcess ("client",
                              "-ORBuseParallelConnects 1 ".
                              "-k file://$client_iorfile");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($server->GetFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($client->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print "LF wait strategy test\n";

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print "\nLF wait strategy, corbaloc test\n";

$CL->Arguments ("-ORBUseSharedProfile 1 -ORBuseParallelConnects 1 ".
                "-k $corbaloc");
$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print "\nReactive wait strategy test\n";

$CL->Arguments ("-ORBSvcConf $client_reactive_conf -ORBuseParallelConnects 1 ".
                "-k file://$client_iorfile");
$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print "\nBlocked wait strategy test\n";

$quick_param = '';
if ($quick) {
    $quick_param = "-t";
}
$CL->Arguments ("-ORBSvcConf $client_blocked_conf -ORBuseParallelConnects 1 ".
                "-k file://$client_iorfile $quick_param");
$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 585);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print "\nNo parallel connect test\n";

$quick_param = '';
if ($quick) {
    $quick_param = "-t";
}
$CL->Arguments ("-ORBuseParallelConnects 0 ".
                "-k file://$client_iorfile $quick_param");
$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 885);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$CL->Arguments ("-ORBuseParallelConnects 1 ".
                "-k file://$client_iorfile -x");
$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
