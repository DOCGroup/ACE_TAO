eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';
$ciao_root = "$ENV{CIAO_ROOT}";

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $target = PerlACE::TestTarget::create_target (1);
my $host = new PerlACE::TestTarget;

$iorbase1 = "server1.ior";
$iorbase2 = "server2.ior";

$server_iorfile1 = $target->LocalFile ($iorbase1);
$server_iorfile2 = $target->LocalFile ($iorbase2);

$target->DeleteFile($iorbase1);
$target->DeleteFile($iorbase2);

$client_iorfile1 = $host->LocalFile ($iorbase1);
$client_iorfile2 = $host->LocalFile ($iorbase2);

$host->DeleteFile($iorbase1);
$host->DeleteFile($iorbase2);

$SV1 = $target->CreateProcess ("$ciao_root/bin/ciao_componentserver", "-ORBDebuglevel $debug_level -u $server_iorfile1 -o $server_iorfile1");
$SV2 = $target->CreateProcess ("$ciao_root/bin/ciao_componentserver", "-ORBDebuglevel $debug_level -u $server_iorfile2 -o $server_iorfile2");

$CL = $host->CreateProcess ("client", "-k file://$client_iorfile1 -j file://$client_iorfile2");

$server_status1 = $SV1->Spawn ();
$server_status2 = $SV2->Spawn ();

if ($server_status1 != 0) {
    print STDERR "ERROR: server1 returned $server_status1\n";
    exit 1;
}
if ($server_status2 != 0) {
    print STDERR "ERROR: server2 returned $server_status2\n";
    exit 1;
}

if ($target->WaitForFileTimed ($iorbase1,
                        $target->ProcessStartWaitInterval ()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile1>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

if ($target->WaitForFileTimed ($iorbase2,
                        $target->ProcessStartWaitInterval ()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile2>\n";
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}

if ($target->GetFile ($server_iorfile1, $iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$iorbase1>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

if ($target->GetFile ($server_iorfile2, $iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$iorbase2>\n";
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval ());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status1 = $SV1->WaitKill ($server1->ProcessStopWaitInterval ());
$server_status2 = $SV2->WaitKill ($server2->ProcessStopWaitInterval ());

if ($server_status1 != 0) {
    print STDERR "ERROR: server1 returned $server_status1\n";
    $status = 1;
}

if ($server_status2 != 0) {
    print STDERR "ERROR: server2 returned $server_status2\n";
    $status = 1;
}

$target->GetStderrLog();

$target->DeleteFile($iorbase1);
$target->DeleteFile($iorbase2);

$host->DeleteFile($iorbase1);
$host->DeleteFile($iorbase2);

exit $status;
