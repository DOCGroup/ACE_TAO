eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

## The LoadManager needs to register signals with the ORB's reactor (on
## Windows only) and thus can not use the TP Reactor since it doesn't
## support that kind of thing.  So, we swith to the Select MT Reactor.
$lm_conf = "windows" . $PerlACE::svcconf_ext;

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $server1 = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $server2 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $client = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

my $ior1file = "lm.ior";
my $ior2file = "obj.ior";

#Files which used by server1
my $server1_ior1file = $server1->LocalFile ($ior1file);
$server1->DeleteFile($ior1file);

#Files which used by server2
my $server2_ior1file = $server2->LocalFile ($ior1file);
$server2->DeleteFile($ior1file);
my $server2_ior2file = $server2->LocalFile ($ior2file);
$server2->DeleteFile($ior2file);

#Files which used by client2
my $client_ior2file = $client->LocalFile ($ior2file);
$client->DeleteFile($ior2file);


$SV1 = $server1->CreateProcess ("../../../../LoadBalancer/tao_loadmanager",
                              "-ORBdebuglevel $debug_level " .
                              "-o $server1_ior1file " .
                              ($^O eq 'MSWin32' ?
                                           " -ORBSvcConf $lm_conf" : ''));

$SV2 = $server2->CreateProcess ("server",
                              "-ORBdebuglevel $debug_level " .
                              "-ORBInitRef LoadManager=file://$server2_ior1file " .
                              "-o $server2_ior2file");

$CL = $client->CreateProcess ("client",
                              "-ORBInitRef LoadManager=file://$client_ior2file");

print STDERR "\n\n======== Running Application Controlled Membership Test================\n";
print STDERR "\n";

print STDERR "This test uses the Random Load Balancing strategy in the Cygnus Load Balancer\n";

print STDERR "6 servers are created and added into a Object Group\n";

print STDERR "When the client makes an invocation, Random load balancing strategy\n";
print STDERR "selects one of the servers and then the client makes 5 invocations on\n";
print STDERR "the server. When done, the application has to delete the object from the\n";
print STDERR "object group. This is called the application controlled membership of\n";
print STDERR "object group.\n";
print STDERR "\n";

$server_status = $SV1->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server1->WaitForFileTimed ($ior1file,
                               $server1->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server1_ior1file>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

if ($server1->GetFile ($ior1file) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server1_ior1file>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

if ($server2->PutFile ($ior2file) == -1) {
    print STDERR "ERROR: cannot set file <$server2_ior2file>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

$server_status = $SV2->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

sub KillServers{
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
}

if ($server2->WaitForFileTimed ($ior2file,
                               $server2->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server2_ior2file>\n";
    KillServers ();
    exit 1;
}

if ($server2->GetFile ($ior2file) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server2_ior2file>\n";
    KillServers ();
    exit 1;
}

if ($client->PutFile ($ior2file) == -1) {
    print STDERR "ERROR: cannot set file <$client_ior2file>\n";
    KillServers ();
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 85);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV2->WaitKill ($server2->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server_status = $SV1->TerminateWaitKill ($server1->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server1->DeleteFile($ior1file);
$server2->DeleteFile($ior1file);
$server2->DeleteFile($ior2file);
$client->DeleteFile($ior2file);

exit $status;
