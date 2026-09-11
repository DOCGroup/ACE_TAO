#!/usr/bin/perl

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $ior_file = "test.ior";
my $status = 0;

my $server = PerlACE::TestTarget::create_target (1) || die "Cannot create server target";
my $client = PerlACE::TestTarget::create_target (2) || die "Cannot create client target";
my $debug_level = '0';
my $cdebug_level = '0';
foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
    if ($i eq '-cdebug') {
      $cdebug_level = '10';
    }
}

my $server_ior = $server->LocalFile ($ior_file);
my $client_ior = $client->LocalFile ($ior_file);

$server->DeleteFile ($ior_file);
$client->DeleteFile ($ior_file);

my $SV = $server->CreateProcess (
    "server",
    "-ORBSvcConf svc.conf -o $server_ior -ORBDebugLevel $debug_level -ORBVerboseLogging 1"
);

my $CL = $client->CreateProcess (
    "client",
    "-ORBSvcConf svc.conf -k file://$client_ior -ORBDebugLevel $cdebug_level -ORBVerboseLogging 1"
);

my $server_status = $SV->Spawn ();
if ($server_status != 0) {
    print STDERR "ERROR: server Spawn returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($ior_file,
                               $server->ProcessStartWaitInterval ()) == -1) {
    print STDERR "ERROR: IOR file '$server_ior' not created\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($server->GetFile ($ior_file) == -1) {
    print STDERR "ERROR: server GetFile '$ior_file' failed\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($client->PutFile ($ior_file) == -1) {
    print STDERR "ERROR: client PutFile '$ior_file' failed\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

my $client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval () + 5);
if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

my $server_exit = $SV->WaitKill ($server->ProcessStopWaitInterval () + 5);
if ($server_exit != 0) {
    print STDERR "ERROR: server returned $server_exit\n";
    $status = 1;
}

$server->DeleteFile ($ior_file);
$client->DeleteFile ($ior_file);

exit $status;
