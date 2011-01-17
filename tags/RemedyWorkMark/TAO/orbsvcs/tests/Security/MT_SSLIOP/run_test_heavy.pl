eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

$threads = '100';
$iterations = '10000';
$number_threads_in_client = '20';

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

#Files which used by client1
my $client1_iorfile = $client1->LocalFile ($iorfile);
$client1->DeleteFile($iorfile);

#Files which used by client2
my $client2_iorfile = $client2->LocalFile ($iorfile);
$client2->DeleteFile($iorfile);

#Configuration files for server and client
$server_conf = $server->LocalFile ("server" . $PerlACE::svcconf_ext);
$client1_conf = $client1->LocalFile ("client" . $PerlACE::svcconf_ext);
$client2_conf = $client2->LocalFile ("client" . $PerlACE::svcconf_ext);


$SV = $server->CreateProcess ("server",
                              "-ORBdebuglevel $debug_level " .
                              "-ORBSvcConf $server_conf " .
                              "-o $server_iorfile " .
                              "-n $threads");

$CL1 = $client1->CreateProcess ("client",
                                "-k file://$client1_iorfile " .
                                "-ORBSvcConf $client1_conf " .
                                "-n $number_threads_in_client -i $iterations");

$CL2 = $client2->CreateProcess ("client",
                                "-k file://$client2_iorfile " .
                                "-ORBSvcConf $client2_conf " .
                                "-n $number_threads_in_client -i $iterations -x");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($iorfile,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    KillServers ();
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

$client_status = $CL1->SpawnWaitKill ($client1->ProcessStartWaitInterval() + 580);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}


$client_status = $CL2->SpawnWaitKill ($client2->ProcessStartWaitInterval() + 580);

if ($client_status != 0) {
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
