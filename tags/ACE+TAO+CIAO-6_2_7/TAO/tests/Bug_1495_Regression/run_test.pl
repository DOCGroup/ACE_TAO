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

my $client = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $server = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";


my $iorbase = "test.ior";
my $thrserverfile = "thr_server.ior";

my $server_iorbase = $server->LocalFile ($iorbase);
my $client_iorbase = $client->LocalFile ($iorbase);

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

my $server_thrserverfile = $server->LocalFile ($thrserverfile);
my $client_thrserverfile = $client->LocalFile ($thrserverfile);
$server->DeleteFile($thrserverfile);
$client->DeleteFile($thrserverfile);

$SV = $server->CreateProcess ("server",
                              "-ORBdebuglevel $debug_level " .
                              "-o $server_iorbase " .
                              "-i file://$server_thrserverfile");

$CL = $client->CreateProcess ("client",
                              "-ORBdebuglevel $debug_level " .
                              "-o $client_thrserverfile " .
                              "-i $client_iorbase");

$client_status = $CL->Spawn ();

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    exit 1;
}

if ($client->WaitForFileTimed ($thrserverfile,
                               $client->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_thrserverfile>\n";
    $CL->Kill (); $CL->TimedWait (1);
    exit 1;
}

if ($client->GetFile ($thrserverfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$client_thrserverfile>\n";
    $CL->Kill (); $CL->TimedWait (1);
    exit 1;
}
if ($server->PutFile ($thrserverfile) == -1) {
    print STDERR "ERROR: cannot set file <$server_thrserverfile>\n";
    $CL->Kill (); $CL->TimedWait (1);
    exit 1;
}

$server_status = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval() + 20);

if ($server_status != 0) {
    print STDERR "ERROR: client returned $server_status\n";
    $status = 1;
}

$client_status = $CL->WaitKill ($client->ProcessStopWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$server->DeleteFile($thrserverfile);
$client->DeleteFile($thrserverfile);

exit $status;
