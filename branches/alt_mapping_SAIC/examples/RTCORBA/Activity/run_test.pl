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

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client1 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $client2 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

my $iorfile = "naming.ior";
my $server_flag_file = "server.flag";

$client2_flag_file = $client2->LocalFile ($server_flag_file);
$client2->DeleteFile ($server_flag_file);

$server_conf = $client2->LocalFile ("server.conf");

$client1_conf = $client1->LocalFile ("client.conf");

#Files which used by server
my $server_iorfile = $server->LocalFile ($iorfile);
$server->DeleteFile($iorfile);

#Files which used by client1
my $client1_iorfile = $client1->LocalFile ($iorfile);
$client1->DeleteFile($iorfile);

#Files which used by client2
my $client2_iorfile = $client2->LocalFile ($iorfile);
$client2->DeleteFile($iorfile);

$SV = $server->CreateProcess ("../../../orbsvcs/Naming_Service/tao_cosnaming",
                              "-ORBdebuglevel $debug_level " .
                              "-o $server_iorfile");

$CL1 = $client1->CreateProcess ("activity",
                                "-ORBInitRef NameService=file://$client1_iorfile " .
                                "-ORBSvcConf $client1_conf");

$CL2 = $client2->CreateProcess ("activity",
                                "-ORBInitRef NameService=file://$client2_iorfile " .
                                "-ORBSvcConf $server_conf " .
                                "-Started_Flag $client2_flag_file");

print STDERR "Running tao_cosnaming\n";

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($iorfile,
                               $server->ProcessStartWaitInterval() + 45) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($server->GetFile ($iorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($client1->PutFile ($iorfile) == -1) {
    print STDERR "ERROR: cannot set file <$client1_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($client2->PutFile ($iorfile) == -1) {
    print STDERR "ERROR: cannot set file <$client2_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print STDERR "Running Activity Server\n";

$client_status = $CL2->Spawn ();

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    exit 1;
}

if ($client2->WaitForFileTimed ($server_flag_file,
                               $client2->ProcessStartWaitInterval() + 45) == -1) {
    print STDERR "ERROR: cannot find file <$client2_flag_file>\n";
    $SV->Kill (); $SV->TimedWait (1);
    $CL2->Kill (); $CL2->TimedWait (1);
    exit 1;
}

print STDERR "Running Activity Client\n";

$client_status = $CL1->SpawnWaitKill ($client1->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$client_status = $CL2->WaitKill ($client2->ProcessStopWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$SV->Kill ();

$client2->DeleteFile ($server_flag_file);
$server->DeleteFile($iorfile);
$client1->DeleteFile($iorfile);
$client2->DeleteFile($iorfile);

exit $status;
