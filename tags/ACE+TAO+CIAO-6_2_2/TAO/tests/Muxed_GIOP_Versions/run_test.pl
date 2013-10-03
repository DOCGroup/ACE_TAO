eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client1 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $client2 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $client3 = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";

my $TARGETHOSTNAME = $server->HostName ();
my $port = $server->RandomPort ();

my $iorbase = "server.ior";
my $logbase = "orb.$port.log";
my $server_iorfile = $server->LocalFile ($iorbase);
my $server_logfile = $server->LocalFile ($logbase);
my $client1_iorfile = $client1->LocalFile ($iorbase);
my $client1_logfile = $client1->LocalFile ($logbase);
my $client2_iorfile = $client2->LocalFile ($iorbase);
my $client2_logfile = $client2->LocalFile ($logbase);
my $client3_iorfile = $client3->LocalFile ($iorbase);
my $client3_logfile = $client3->LocalFile ($logbase);
$server->DeleteFile($iorbase);
$server->DeleteFile($logbase);
$client1->DeleteFile($iorbase);
$client1->DeleteFile($logbase);
$client2->DeleteFile($iorbase);
$client2->DeleteFile($logbase);
$client3->DeleteFile($iorbase);
$client3->DeleteFile($logbase);

# Testing Options:
# most effective is $serveriterations=1000 (with or without clients)
my $verbose = 1;
my $serverthreads = '10';
my $selfabusethreads = '5';
my $clientthreads = '5';
my $serveriterations = 1000;
my $clientiterations = 1000;
my $clients = 1;

# use GIOP 1.0
my $corbaloc_str = "corbaloc:iiop:1.0\@$TARGETHOSTNAME:$port/".
    "SomeObjectNameThatDontExist";

# -ORBDebugLevel 10 seems to encourage the problem
# -ORBCollocation no is required for server to produce the problem

$SV = $server->CreateProcess ("server",
                              "-ORBCollocation no -ORBdebuglevel 10 ".
                              "-ORBLogFile $server_logfile ".
                              "-ORBListenEndpoints iiop://$TARGETHOSTNAME:$port ".
                              "-i $serveriterations -n $serverthreads ".
                              "-c $selfabusethreads -l $corbaloc_str ".
                              "-o $server_iorfile");
$CL1 = $client1->CreateProcess ("client",
                                "-ORBdebuglevel 10 -ORBLogFile $client1_logfile ".
                                "-l $corbaloc_str -k file://$client1_iorfile ".
                                "-n $clientthreads -i $clientiterations");
$CL2 = $client2->CreateProcess ("client",
                                "-ORBdebuglevel 10 -ORBLogFile $client2_logfile ".
                                "-l $corbaloc_str -k file://$client2_iorfile ".
                                "-n $clientthreads -i $clientiterations");
$CL3 = $client3->CreateProcess ("client",
                                "-ORBdebuglevel 10 -ORBLogFile $client3_logfile ".
                                "-l $corbaloc_str -k file://$client3_iorfile ".
                                "-n $clientthreads -i $clientiterations");

print STDERR "***** Start the server*** \n" if $verbose;

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

# leave server reap some self abuse before clients start abusing it
if ($serveriterations > 20) {
    $SV->TimedWait(10);
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

print STDERR "****** Start the clients*** \n" if $verbose;

if ($clients > 0) {
    $client_status = $CL1->Spawn ();

    if ($client_status != 0) {
        print STDERR "ERROR: client returned $client_status\n";
        $SV->Kill (); $SV->TimedWait (1);
        exit 1;
    }
}

if ($clients > 1) {
    $client_status = $CL2->Spawn ();

    if ($client_status != 0) {
        print STDERR "ERROR: client returned $client_status\n";
        $SV->Kill (); $SV->TimedWait (1);
        $CL1->Kill (); $CL1->TimedWait (1);
        exit 1;
    }
}

if ($clients > 2) {
    $client_status = $CL3->Spawn ();

    if ($client_status != 0) {
        print STDERR "ERROR: client returned $client_status\n";
        $SV->Kill (); $SV->TimedWait (1);
        $CL1->Kill (); $CL1->TimedWait (1);
        $CL2->Kill (); $CL2->TimedWait (1);
        exit 1;
    }
}

if ($clients > 0) {
    $client_status = $CL1->WaitKill ($client1->ProcessStopWaitInterval() + 195);

    if ($client_status != 0) {
        print STDERR "ERROR: client 1 returned $client_status\n";
        $status = 1;
    }
}

if ($clients > 1) {
    $client_status = $CL2->WaitKill ($client2->ProcessStopWaitInterval());

    if ($client_status != 0) {
        print STDERR "ERROR: client 2 returned $client_status\n";
        $status = 1;
    }
}

if ($clients > 2) {
    $client_status = $CL3->WaitKill ($client3->ProcessStopWaitInterval());

    if ($client_status != 0) {
        print STDERR "ERROR: client 3 returned $client_status\n";
        $status = 1;
    }
}

$CL1->Arguments ("-l $corbaloc_str -i 1 -x ".
                 "-k file://$client1_iorfile");

$client_status = $CL1->SpawnWaitKill ($client1->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval() + 135);

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$server->DeleteFile($logbase);
$client1->DeleteFile($iorbase);
$client1->DeleteFile($logbase);
$client2->DeleteFile($iorbase);
$client2->DeleteFile($logbase);
$client3->DeleteFile($iorbase);
$client3->DeleteFile($logbase);

exit $status;
