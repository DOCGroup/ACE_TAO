eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';
$data_bytes = '10';
$iterations = '10';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "server.ior";
my $done_file = "client_done.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);

my $client_done_file = $client->LocalFile ($done_file);

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$client->DeleteFile($done_file);

$SV = $server->CreateProcess ("server",
                              "-ORBdebuglevel $debug_level " .
                              "-o $server_iorfile " .
                              "-s $client_done_file");

$CL = $client->CreateProcess ("client",
                              "-k file://$client_iorfile " .
                              "-d $data_bytes -i $iterations -x");

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

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 185);

if ($client_status != 0) {
    $time = localtime;
    print STDERR "ERROR: client returned $client_status at $time\n";
    $status = 1;
}

open (FILE , ">$client_done_file");
print FILE "Client Done !\n";
close (FILE);


$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval() + 390);

if ($server_status != 0) {
    $time = localtime;
    print STDERR "ERROR: server returned $server_status at $time\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);
$client->DeleteFile($done_file);

exit $status;
