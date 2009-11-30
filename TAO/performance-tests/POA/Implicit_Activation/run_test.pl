eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

$iterations = '10000';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

print STDERR "================ Collocated Activation Test\n";

my $server1 = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $server2 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $client = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";


my $iorbase = "test.ior";
my $server_iorfile = $server2->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server2->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$CO = $server1->CreateProcess ("collocated", "-i $iterations");
$SV = $server2->CreateProcess ("server", "-ORBdebuglevel $debug_level -o $server_iorfile");
$CL = $client->CreateProcess ("client", "-k file://$client_iorfile " .
                              "-i $iterations");

$server_status = $CO->SpawnWaitKill ($server1->ProcessStartWaitInterval() + 100);

if ($client_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

print STDERR "================ Activation Latency Test\n";

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server2->WaitForFileTimed ($iorbase,
                               $server2->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($server2->GetFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($client->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 100);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server2->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server2->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
