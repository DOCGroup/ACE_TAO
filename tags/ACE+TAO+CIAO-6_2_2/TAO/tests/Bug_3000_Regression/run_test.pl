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

my $iorfile = "test.ior";

#Files which used by server
my $server_iorfile = $server->LocalFile ($iorfile);
$server->DeleteFile($iorfile);

my $base_conf = "rw" . $PerlACE::svcconf_ext;
my $server_conf = $server->LocalFile ($base_conf);

if ($server->PutFile ($base_conf) == -1) {
    print STDERR "ERROR: cannot set file <$server_conf>\n";
    exit 1;
}

#Files which used by client1
my $client1_iorfile = $client1->LocalFile ($iorfile);
$client1->DeleteFile($iorfile);

#Files which used by client2
my $client2_iorfile = $client2->LocalFile ($iorfile);
$client2->DeleteFile($iorfile);

$SV = $server->CreateProcess ("server",
                              "-ORBdebuglevel $debug_level " .
                              "-ORBSvcConf $server_conf " .
                              "-o $server_iorfile");

$port1 = $client1->RandomPort ();

$CL1 = $client1->CreateProcess ("client",
                                "-k file://$client1_iorfile " .
                                "-ORBDottedDecimalAddresses 1 " .
                                "-ORBEndpoint iiop://:$port1");

$port2 = $client2->RandomPort ();

$CL2 = $client2->CreateProcess ("client",
                                "-s " .
                                "-k file://$client2_iorfile " .
                                "-ORBDottedDecimalAddresses 2 " .
                                "-ORBEndpoint iiop://:$port2");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($iorfile,
                               $server->ProcessStartWaitInterval()) == -1) {
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

$client_status = $CL1->SpawnWaitKill ($client1->ProcessStartWaitInterval() + 45);

# The client crashes, therefore it normally exists with status != 0,
# but a status of -1 would be a serious error.
if ($client_status == -1) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$client_status = $CL2->SpawnWaitKill ($client2->ProcessStartWaitInterval() + 45);

# The client crashes, therefore it normally exists with status != 0,
# but a status of -1 would be a serious error.
if ($client_status == -1) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorfile);
$client1->DeleteFile($iorfile);
$client2->DeleteFile($iorfile);

exit $status;
