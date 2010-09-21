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

my $server1 = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $server2 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $client = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

$client->AddLibPath ('../lib');

my $ior1file = "naming.ior";
my $ior2file = "notify.ior";

#Files which used by server1
my $server1_ior1file = $server1->LocalFile ($ior1file);
$server1->DeleteFile($ior1file);

#Files which used by server2
my $server2_ior2file = $server2->LocalFile ($ior2file);
$server2->DeleteFile($ior2file);

$hostname = $server1->HostName ();
$port = $server1->RandomPort ();

$SV1 = $server1->CreateProcess ("../../../Naming_Service/Naming_Service",
                              "-ORBdebuglevel $debug_level " .
                              "-ORBEndpoint iiop://$hostname:$port " .
                              "-o $server1_ior1file");

$SV2 = $server2->CreateProcess ("../../../Notify_Service/tao_cosnotification",
                              "-ORBdebuglevel $debug_level " .
                              "-ORBInitRef NameService=iioploc://$hostname:$port/NameService " .
                              "-IORoutput $server2_ior2file " .
                              "-Channel -ChannelName Channel1 -ChannelName Channel2");

$CL = $client->CreateProcess ("Consumer",
                              "-ORBdebuglevel $debug_level " .
                              "-ORBInitRef NameService=iioploc://$hostname:$port/NameService");

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


$server_status = $SV2->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server2->WaitForFileTimed ($ior2file,
                               $server2->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server2_ior2file>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$SV2->Kill ();
$SV1->Kill ();

$server1->DeleteFile($ior1file);
$server2->DeleteFile($ior2file);

exit $status;
