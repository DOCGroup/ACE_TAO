eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

$client_wait_time = 15;


foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client1 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $client2 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $client3 = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client1_iorfile = $client1->LocalFile ($iorbase);
my $client2_iorfile = $client2->LocalFile ($iorbase);
my $client3_iorfile = $client3->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client1->DeleteFile($iorbase);
$client2->DeleteFile($iorbase);
$client3->DeleteFile($iorbase);

#Config files
$server_conf = $server->LocalFile ("server" . $PerlACE::svcconf_ext);
$client1_conf = $client1->LocalFile ("client" . $PerlACE::svcconf_ext);
$client2_conf = $client2->LocalFile ("client" . $PerlACE::svcconf_ext);
$client3_conf = $client3->LocalFile ("client" . $PerlACE::svcconf_ext);

$port = $server->RandomPort ();

# Set the SSL environment
# This doesn't work on Windows.  For some reason,
# environment variables aren't propagated to child processes.
#$ENV{'SSL_CERT_FILE'} = 'cacert.pem';

$SV = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level " .
                              "-ORBSvcConf $server_conf " .
                              "-ORBdotteddecimaladdresses 1 " .
                              "-ORBEndpoint iiop://:/ssl_port=$port " .
                              "-o $server_iorfile");

$CL1 = $client1->CreateProcess ("client",
                                "-ORBSvcConf $client1_conf " .
                                "-w $client_wait_time " .
                                "-s " .
                                "-k file://$client1_iorfile");

$CL2 = $client2->CreateProcess ("client",
                                "-ORBSvcConf $client2_conf " .
                                "-w $client_wait_time " .
                                "-k file://$client2_iorfile");

$CL3 = $client3->CreateProcess ("client",
                                "-ORBSvcConf $client3_conf " .
                                "-w $client_wait_time " .
                                "-k file://$client3_iorfile");


local $start_time = time();
local $max_running_time = 500; # < 10 minutes
local $elapsed = time() - $start_time;

if ($ARGV[0] eq '-quick')  {
    $elapsed = 0;
    $max_running_time = 1;
}

while($status == 0 && $elapsed < $max_running_time){

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

    # Start all clients in parallel
    $client_status = $CL1->Spawn ();

    if ($client_status != 0) {
        print STDERR "ERROR: client #1 returned $client_status\n";
        $SV->Kill (); $SV->TimedWait (1);
        $CL1->Kill (); $CL1->TimedWait (1);
        exit 1;
    }

    $client_status = $CL2->Spawn ();

    if ($client_status != 0) {
        print STDERR "ERROR: client #2 returned $client_status\n";
        $SV->Kill (); $SV->TimedWait (1);
        $CL1->Kill (); $CL1->TimedWait (1);
        $CL2->Kill (); $CL2->TimedWait (1);
        exit 1;
    }

    $client_status = $CL3->Spawn ();

    if ($client_status != 0) {
        print STDERR "ERROR: client #2 returned $client_status\n";
        $SV->Kill (); $SV->TimedWait (1);
        $CL1->Kill (); $CL1->TimedWait (1);
        $CL2->Kill (); $CL2->TimedWait (1);
        $CL3->Kill (); $CL3->TimedWait (1);
        exit 1;
    }

    # Let our clients to execute few requests
    sleep (3);

    # Now kill the server and start it again.
    $SV->Kill ();
    $SV->TimedWait (1);

    sleep (5);

    $server->DeleteFile($iorbase);
    $client1->DeleteFile($iorbase);
    $client2->DeleteFile($iorbase);
    $client3->DeleteFile($iorbase);


    $server_status = $SV->Spawn ();

    if (0 != $server_status){
      print STDERR "ERROR: Restarting server returned $server_status\n";
      $CL3->Kill ();
      $CL2->Kill ();
      $CL1->Kill ();
      exit 1;
    }

    if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$server_iorfile>\n";
        $SV->Kill (); $SV->TimedWait (1);
        $CL1->Kill (); $CL1->TimedWait (1);
        $CL2->Kill (); $CL2->TimedWait (1);
        $CL3->Kill (); $CL3->TimedWait (1);
        exit 1;
    }

    if ($server->GetFile ($iorbase) == -1) {
        print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
        $SV->Kill (); $SV->TimedWait (1);
        $CL1->Kill (); $CL1->TimedWait (1);
        $CL2->Kill (); $CL2->TimedWait (1);
        $CL3->Kill (); $CL3->TimedWait (1);
        exit 1;
    }
    if ($client1->PutFile ($iorbase) == -1) {
        print STDERR "ERROR: cannot set file <$client1_iorfile>\n";
        $SV->Kill (); $SV->TimedWait (1);
        $CL1->Kill (); $CL1->TimedWait (1);
        $CL2->Kill (); $CL2->TimedWait (1);
        $CL3->Kill (); $CL3->TimedWait (1);
        exit 1;
    }
    if ($client2->PutFile ($iorbase) == -1) {
        print STDERR "ERROR: cannot set file <$client2_iorfile>\n";
        $SV->Kill (); $SV->TimedWait (1);
        $CL1->Kill (); $CL1->TimedWait (1);
        $CL2->Kill (); $CL2->TimedWait (1);
        $CL3->Kill (); $CL3->TimedWait (1);
        exit 1;
    }
    if ($client3->PutFile ($iorbase) == -1) {
        print STDERR "ERROR: cannot set file <$client3_iorfile>\n";
        $SV->Kill (); $SV->TimedWait (1);
        $CL1->Kill (); $CL1->TimedWait (1);
        $CL2->Kill (); $CL2->TimedWait (1);
        $CL3->Kill (); $CL3->TimedWait (1);
        exit 1;
    }

    # Wait for the server and clients to finish

    # First client waits for others and shutdowns the server.
    $client_status = $CL1->WaitKill ($client1->ProcessStopWaitInterval() + 35);

    if (0 != $client_status){
      print STDERR "ERROR: Stopping client #1 returned $client_status\n";
      $status = 1;
    }

    $client_status = $CL2->WaitKill ($client2->ProcessStopWaitInterval() + 15);

    if (0 != $client_status){
      print STDERR "ERROR: Stopping client #2 returned $client_status\n";
      $status = 1;
    }

    $client_status = $CL3->WaitKill ($client3->ProcessStopWaitInterval() + 15);

    if (0 != $client_status){
      print STDERR "ERROR: Stopping client #3 returned $client_status\n";
      $status = 1;
    }

    $server_status = $SV->WaitKill ($server->ProcessStopWaitInterval() + 15);

    if ($server_status != 0) {
        print STDERR "ERROR: server returned $server_status\n";
        $status = 1;
    }

    $server->DeleteFile($iorbase);
    $client1->DeleteFile($iorbase);
    $client2->DeleteFile($iorbase);
    $client3->DeleteFile($iorbase);

    $elapsed = time() - $start_time;
}

$server->DeleteFile($iorbase);
$client1->DeleteFile($iorbase);
$client2->DeleteFile($iorbase);
$client3->DeleteFile($iorbase);

exit $status;
