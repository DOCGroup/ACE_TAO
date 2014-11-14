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
my $client1 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $client2 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $client3 = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client1_iorfile = $client1->LocalFile ($iorbase);
my $client2_iorfile = $client2->LocalFile ($iorbase);
my $client3_iorfile = $client3->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client1->DeleteFile($iorbase);
$client2->DeleteFile($iorbase);
$client3->DeleteFile($iorbase);

# Initialize and copy the client configuration files
my $client2_conf_base = "reactor$PerlACE::svcconf_ext";
my $client2_conf = $client2->LocalFile ($client2_conf_base);

if ($client2->PutFile ($client2_conf_base) == -1) {
    print STDERR "ERROR: cannot set file <$client2_conf>\n";
    exit 1;
}

my $client3_conf_base = "blocked$PerlACE::svcconf_ext";
my $client3_conf = $client3->LocalFile ($client3_conf_base);

if ($client3->PutFile ($client3_conf_base) == -1) {
    print STDERR "ERROR: cannot set file <$client3_conf>\n";
    exit 1;
}

$SV = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level -o $server_iorfile");
$CL1 = $client1->CreateProcess ("client", "-k file://$client1_iorfile -x");
# Use a single thread in this test, because the strategy is only for
# single threaded cases
$CL2 = $client2->CreateProcess ("client", "-ORBSvcConf $client2_conf -n 1 -k file://$client2_iorfile");
$CL3 = $client3->CreateProcess ("client", "-ORBSvcConf $client3_conf -k file://$client3_iorfile");

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
if ($client1->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client1_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($client2->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client2_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($client3->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client3_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print STDERR "===== Base test, using LF\n";

$client_status = $CL1->SpawnWaitKill ($client1->ProcessStartWaitInterval() + 85);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print STDERR "===================================\n";
print STDERR "===== Test, using reactor to connect\n";

$client_status = $CL2->SpawnWaitKill ($client2->ProcessStartWaitInterval() + 85);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print STDERR "=========================================\n";
print STDERR "===== Test, using blocked connect strategy\n";

$client_status = $CL3->SpawnWaitKill ($client3->ProcessStartWaitInterval() + 85);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client1->DeleteFile($iorbase);
$client2->DeleteFile($iorbase);
$client3->DeleteFile($iorbase);

exit $status;
