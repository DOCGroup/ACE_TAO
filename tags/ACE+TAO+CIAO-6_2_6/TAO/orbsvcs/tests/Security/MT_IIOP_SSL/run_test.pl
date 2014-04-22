eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

$threads = '4';
$iterations = '100';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client1 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $client2 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $client3 = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $client4 = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client1_iorfile = $client1->LocalFile ($iorbase);
my $client2_iorfile = $client2->LocalFile ($iorbase);
my $client3_iorfile = $client3->LocalFile ($iorbase);
my $client4_iorfile = $client4->LocalFile ($iorbase);

$server->DeleteFile($iorbase);
$client1->DeleteFile($iorbase);
$client2->DeleteFile($iorbase);
$client3->DeleteFile($iorbase);
$client4->DeleteFile($iorbase);

#Config files
$server_conf = $server->LocalFile ("server" . $PerlACE::svcconf_ext);
$client1_conf = $client1->LocalFile ("client" . $PerlACE::svcconf_ext);
$client2_conf = $client2->LocalFile ("client" . $PerlACE::svcconf_ext);
$client3_conf = $client3->LocalFile ("client" . $PerlACE::svcconf_ext);
$client4_conf = $client4->LocalFile ("client" . $PerlACE::svcconf_ext);

# Set the SSL environment
# This doesn't work on Windows.  For some reason,
# environment variables aren't propagated to child processes.
#$ENV{'SSL_CERT_FILE'} = 'cacert.pem';

$SV = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level " .
                              "-ORBSvcConf $server_conf " .
                              "-o $server_iorfile " .
                              "-n $threads");

$CL1 = $client1->CreateProcess ("client",
                                "-ORBSvcConf $client1_conf " .
                                "-n $threads " .
                                "-i $iterations " .
                                "-k file://$client1_iorfile");

$CL2 = $client2->CreateProcess ("client",
                                "-ORBSvcConf $client2_conf " .
                                "-n $threads " .
                                "-i $iterations " .
                                "-k file://$client2_iorfile");

$CL3 = $client3->CreateProcess ("client",
                                "-ORBSvcConf $client3_conf " .
                                "-n $threads " .
                                "-i $iterations " .
                                "-k file://$client3_iorfile");

$CL4 = $client4->CreateProcess ("client",
                                "-ORBSvcConf $client4_conf " .
                                "-n $threads " .
                                "-i $iterations " .
                                "-x " .
                                "-k file://$client4_iorfile");

# Start the server
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
if ($client1->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client1_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($client2->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client2_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($client3->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client3_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($client4->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client4_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}


$CL1->Spawn ();
$CL2->Spawn ();
$CL3->Spawn ();


$client_status = $CL1->WaitKill ($client1->ProcessStopWaitInterval() + 50);

if (0 != $client_status){
    print STDERR "ERROR: Stopping client #1 returned $client_status\n";
    $status = 1;
}

$client_status = $CL2->WaitKill ($client2->ProcessStopWaitInterval() + 50);

if (0 != $client_status){
    print STDERR "ERROR: Stopping client #2 returned $client_status\n";
    $status = 1;
}
$client_status = $CL3->WaitKill ($client3->ProcessStopWaitInterval() + 50);

if (0 != $client_status){
    print STDERR "ERROR: Stopping client #3 returned $client_status\n";
    $status = 1;
}

$client_status = $CL4->SpawnWaitKill ($client4->ProcessStartWaitInterval()+ 45);

if ($client_status != 0) {
    print STDERR "ERROR: client #4 returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}


$server->DeleteFile($iorbase);
$client1->DeleteFile($iorbase);
$client2->DeleteFile($iorbase);
$client3->DeleteFile($iorbase);
$client4->DeleteFile($iorbase);

exit $status;
