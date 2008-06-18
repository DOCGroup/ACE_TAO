eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    } 
}

#my $target = PerlACE::TestTarget::create_target ($PerlACE::TestConfig);
my $server = PerlACE::TestTarget::create_target ("server");
#my $client = new PerlACE::TestTarget;
my $client = PerlACE::TestTarget::create_target ("client");
if (!defined $server || !defined $client) {
    exit 1;
}

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($server_iorfile);
$client->DeleteFile($client_iorfile);

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "-ORBDebuglevel $debug_level -o $iorbase");
}
else {
    $SV = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level -o $server_iorfile");
}
$CL = $client->CreateProcess ("client", "-k file://$client_iorfile");
    
$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($server_iorfile,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
} 

if ($server->GetFile ($server_iorfile, $iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
} 
if ($client->PutFile ($iorbase, $client_iorfile) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
} 

$client_status = $CL->SpawnWaitKill (300);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->GetStderrLog();
$client->GetStderrLog();

$server->DeleteFile($server_iorfile);
$client->DeleteFile($client_iorfile);

exit $status;
