eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use PerlACE::TestTarget;

my $target = PerlACE::TestTarget::create_target ("server");
my $host = PerlACE::TestTarget::create_target ("client");

$iorbase = "server.ior";
$server_iorfile = $target->LocalFile ("$iorbase");
$client_iorfile = $host->LocalFile ("$iorbase");
$target->DeleteFile ($server_iorfile);
$host->DeleteFile ($client_iorfile);
$status = 0;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "-o $iorbase");
}
else {
    $SV = $target->CreateProcess ("server", "-o $server_iorfile");
}
$CL = $host->CreateProcess ("client", "-k file://$client_iorfile");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($target->WaitForFileTimed ($server_iorfile,
                               $target->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($target->GetFile ($server_iorfile, $iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
} 
if ($host->PutFile ($iorbase, $client_iorfile) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
} 

$client = $CL->SpawnWaitKill (60);

# The client crashes, therefore it normally exists with status != 0,
# but a status of -1 would be a serious error.
if ($client == -1) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (60);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

$target->GetStderrLog();
$target->DeleteFile ($server_iorfile);
$host->DeleteFile($client_iorfile);

exit $status;
