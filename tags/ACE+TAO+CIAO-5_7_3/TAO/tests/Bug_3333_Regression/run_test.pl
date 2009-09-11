eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "server.ior";
my $forward_forever = "forward_forever.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($forward_forever);
$server->DeleteFile($iorbase);
$client->DeleteFile($forward_forever);

$SV = $server->CreateProcess ("Bug3333_Server", "-o $server_iorfile -p $client_iorfile -ORBObjRefStyle URL -ORBCollocation NO");
$CL = $client->CreateProcess ("Bug3333_Client", "-ORBDebugLevel0 -k file://$server_iorfile -l file://$client_iorfile");

print "Spawn server\n";
$server_status = $SV->Spawn ();
if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}
print "Wait for server ior file\n";
if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()+10) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($server->GetFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($server->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($server->WaitForFileTimed ($forward_forever,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$client_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($server->GetFile ($forward_forever) == -1) {
    print STDERR "ERROR: cannot retrieve file <$client_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($server->PutFile ($forward_forever) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print "Spawn client\n";
$client_status = $CL->SpawnWaitKill (60); # 60= 20x the actual roundtrip timeout policy value of 3sec, just to be sure.
if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SHUTDOWN = $client->CreateProcess ("Bug3333_Client", " -k file://$server_iorfile -s");
    $SHUTDOWN->SpawnWaitKill ($client->ProcessStartWaitInterval()+$client->ProcessStopWaitInterval()+10);
    $status = 1;
}

print "Wait for server to finish\n";
$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval()+10);
if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($forward_forever);

exit $status;
