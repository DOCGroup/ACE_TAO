eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

# List of the proccess which must be killed before exit with error
@kill_list = ();

sub add_to_kills{
    $goal = shift;
    @kill_list = (@kill_list, $goal);
}
sub exit_and_kill{
    $status = shift;
    foreach $goal (@kill_list){
        $goal->Kill (); $goal->TimedWait (1);
    }
    exit $status;
}

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $server1 = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $server2 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $server3 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $server4 = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $client = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";

my $naming_ior = "ns.ior";
my $iorfile = "Messenger.ior";
my $notify_ior = "notify.ior";

my $server2_conf = $server2->LocalFile ("NotSvc.conf");

#Files which used by server1
my $server1_naming_ior = $server1->LocalFile ($naming_ior);
$server1->DeleteFile($naming_ior);

#Files which used by server2
my $server2_naming_ior = $server2->LocalFile ($naming_ior);
my $server2_notify_ior = $server2->LocalFile ($notify_ior);
$server2->DeleteFile($naming_ior);
$server2->DeleteFile($notify_ior);

#Files which used by server3
my $server3_naming_ior = $server3->LocalFile ($naming_ior);
my $server3_iorfile = $server3->LocalFile ($iorfile);
$server3->DeleteFile($naming_ior);
$server3->DeleteFile($iorfile);

#Files which used by server4
my $server4_naming_ior = $server4->LocalFile ($naming_ior);
$server4->DeleteFile($naming_ior);

#Files which used by client
my $client_naming_ior = $client->LocalFile ($naming_ior);
my $client_iorfile = $client->LocalFile ($iorfile);
$client->DeleteFile($iorfile);
$client->DeleteFile($naming_ior);

$hostname = $server1->HostName ();
$port = $server1->RandomPort ();

$SV1 = $server1->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Naming_Service/Naming_Service",
                              "-o $server1_naming_ior");

$SV2 = $server2->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Notify_Service/Notify_Service",
                              "-ORBInitRef NameService=file://$server2_naming_ior " .
                              "-IORoutput $server2_notify_ior " .
                              "-UseSeparateDispatchingORB 1 " .
                              "-ORBSvcConf $server2_conf");

$SV3 = $server3->CreateProcess ("MessengerServer",
                              "-ORBInitRef NameService=file://$server3_naming_ior " .
                              "-o $server3_iorfile");

$SV4 = $server4->CreateProcess ("MessengerConsumer",
                              "-ORBInitRef NameService=file://$client_naming_ior " .
                              "-ORBEndPoint iiop://$hostname:$port " .
                              "-p 1");


$CL = $client->CreateProcess ("MessengerClient",
                              "-ORBInitRef NameService=file://$client_naming_ior " .
                              "-k file://$client_iorfile");

# start Naming Service
$server_status = $SV1->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

add_to_kills ($SV1);

if ($server1->WaitForFileTimed ($naming_ior,
                               $server1->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server1_naming_ior>\n";
    exit_and_kill (1);
}

if ($server1->GetFile ($naming_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server1_naming_ior>\n";
    exit_and_kill (1);
}

if ($server2->PutFile ($naming_ior) == -1) {
    print STDERR "ERROR: cannot set file <$server2_naming_ior>\n";
    exit_and_kill (1);
}
if ($server3->PutFile ($naming_ior) == -1) {
    print STDERR "ERROR: cannot set file <$server3_naming_ior>\n";
    exit_and_kill (1);
}
if ($server4->PutFile ($naming_ior) == -1) {
    print STDERR "ERROR: cannot set file <$server4_naming_ior>\n";
    exit_and_kill (1);
}
if ($client->PutFile ($naming_ior) == -1) {
    print STDERR "ERROR: cannot set file <$client_naming_ior>\n";
    exit_and_kill (1);
}

# start Notification Service

$server_status = $SV2->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit_and_kill (1);
}

add_to_kills ($SV2);

if ($server2->WaitForFileTimed ($notify_ior,
                               $server2->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server2_notify_ior>\n";
    exit_and_kill (1);
}

# start MessengerServer
$server_status = $SV3->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit_and_kill (1);
}

add_to_kills ($SV3);

if ($server3->WaitForFileTimed ($iorfile,
                               $server3->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server3_iorfile>\n";
    exit_and_kill (1);
}

if ($server3->GetFile ($iorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server3_iorfile>\n";
    exit_and_kill (1);
}

if ($client->PutFile ($iorfile) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    exit_and_kill (1);
}

# start first MessengerConsumer
$server_status = $SV4->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit_and_kill (1);
}

add_to_kills ($SV4);

# start MessengerClient
$client_status = $CL->Spawn ();

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    exit_and_kill (1);
}
# wait for first MessengerConsumer to end
$SV4->WaitKill ($server4->ProcessStopWaitInterval());

# start second MessengerConsumer
$SV4->Arguments ("-ORBInitRef NameService=file://$client_naming_ior " .
                 "-ORBEndPoint iiop://$hostname:$port " .
                 "-p 2");
$server_status = $SV4->SpawnWaitKill ($server4->ProcessStartWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$CL->Kill ();
$SV3->Kill ();
$SV2->Kill ();
$SV1->Kill ();

# Remove files
$server1->DeleteFile($naming_ior);
$server2->DeleteFile($naming_ior);
$server2->DeleteFile($notify_ior);
$server3->DeleteFile($naming_ior);
$server3->DeleteFile($iorfile);
$server4->DeleteFile($naming_ior);
$client->DeleteFile($iorfile);
$client->DeleteFile($naming_ior);

exit $status;
