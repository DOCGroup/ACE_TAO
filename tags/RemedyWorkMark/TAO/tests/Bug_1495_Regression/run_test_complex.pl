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

#Schema of the interaction:
#  'Client' <---> 'Middle Server' <---> 'Server'

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $middle = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

my $inputfile = "test.ior";
my $thrserverfile = "thr_server.ior";
my $middleserverfile = "middle.ior";

#Files which used by client
my $client_inputfile = $client->LocalFile ($inputfile);
my $client_thrserverfile = $client->LocalFile ($thrserverfile);
$client->DeleteFile($inputfile);
$client->DeleteFile($thrserverfile);

#Files which used by server
my $server_inputfile = $server->LocalFile ($inputfile);
my $server_middleserverfile = $server->LocalFile ($middleserverfile);
$server->DeleteFile($inputfile);
$server->DeleteFile($middleserverfile);

#Files which used by middle server
my $middle_thrserverfile = $middle->LocalFile ($thrserverfile);
my $middle_middleserverfile = $middle->LocalFile ($middleserverfile);
$middle->DeleteFile($middleserverfile);
$middle->DeleteFile($thrserverfile);

$SV = $server->CreateProcess ("server",
                              "-ORBdebuglevel $debug_level " .
                              "-o $server_inputfile " .
                              "-i file://$server_middleserverfile");

$CL = $client->CreateProcess ("client",
                              "-ORBdebuglevel $debug_level " .
                              "-i file://$client_inputfile " .
                              "-o $client_thrserverfile");

$ML = $middle->CreateProcess ("server",
                              "-ORBdebuglevel $debug_level " .
                              "-i file://$middle_thrserverfile " .
                              "-o $middle_middleserverfile");

$client_status = $CL->Spawn ();

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    exit 1;
}

if ($client->WaitForFileTimed ($thrserverfile,
                               $client->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$client_thrserverfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($client->GetFile ($thrserverfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$client_thrserverfile>\n";
    $CL->Kill (); $CL->TimedWait (1);
    exit 1;
}
if ($middle->PutFile ($thrserverfile) == -1) {
    print STDERR "ERROR: cannot set file <$middle_thrserverfile>\n";
    $CL->Kill (); $CL->TimedWait (1);
    exit 1;
}

$middle_status = $ML->Spawn ();

if ($middle_status != 0) {
    print STDERR "ERROR: client returned <$middle_status>\n";
    exit 1;
}

if ($middle->WaitForFileTimed ($middleserverfile,
                               $client->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$middle_middleserverfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($middle->GetFile ($middleserverfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$middle_middleserverfile>\n";
    $CL->Kill (); $CL->TimedWait (1);
    exit 1;
}
if ($server->PutFile ($middleserverfile) == -1) {
    print STDERR "ERROR: cannot set file <$server_middleserverfile>\n";
    $CL->Kill (); $CL->TimedWait (1);
    exit 1;
}

$server_status = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval() + 35);

if ($server_status != 0) {
    print STDERR "ERROR: client returned $server_status\n";
    $status = 1;
}

$client_status = $CL->WaitKill ($client->ProcessStopWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$middle_status = $ML->WaitKill ($middle->ProcessStopWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $middle_status\n";
    $status = 1;
}

$client->DeleteFile($inputfile);
$client->DeleteFile($thrserverfile);

$server->DeleteFile($inputfile);
$server->DeleteFile($middleserverfile);

$middle->DeleteFile($middleserverfile);
$middle->DeleteFile($thrserverfile);

exit $status;
