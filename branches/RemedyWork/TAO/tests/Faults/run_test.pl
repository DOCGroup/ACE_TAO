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
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $middle = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

my $iorbase = "server.ior";
my $midiorbase = "middle.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
my $client_midiorfile = $client->LocalFile ($midiorbase);
my $middle_iorfile = $middle->LocalFile ($iorbase);
my $middle_midiorfile = $middle->LocalFile ($midiorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);
$client->DeleteFile($midiorbase);
$middle->DeleteFile($iorbase);
$middle->DeleteFile($midiorbase);

my $svcconf = "server$PerlACE::svcconf_ext";
my $server_svcfile = $server->LocalFile ($svcconf);
if ($server->PutFile ($svcconf) == -1) {
    print STDERR "ERROR: cannot set file <$server_svcfile>\n";
    exit 1;
}


###############################################################################
print STDERR "################ Default ORB Config###############\n";
print STDERR "===== Base test, no crashes\n";

$SV = $server->CreateProcess ("server",
                              "-ORBdebuglevel $debug_level ".
                              "-o $server_iorfile");
$CL = $client->CreateProcess ("client", "-k file://$client_iorfile -i 100");

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
if ($client->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

###############################################################################
print STDERR "===== Client crash (abort) during upcall\n";

$CL->Arguments ("-k file://$client_iorfile -i 100 -s");

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status == -1) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

###############################################################################
print STDERR "===== Client crash during upcall\n";

$CL->Arguments ("-k file://$client_iorfile -i 100 -z");

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status == -1) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

###############################################################################
print STDERR "===== Server crash (abort) during upcall\n";

$CL->Arguments ("-k file://$client_iorfile -i 100 -a");

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status == -1) {
    print STDERR "ERROR: server returned $server_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

###############################################################################
print STDERR "===== Server crash during upcall\n";

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
if ($client->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$CL->Arguments ("-k file://$client_iorfile -i 100 -c");

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status == -1) {
    print STDERR "ERROR: server returned $server_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

###############################################################################
print STDERR "===== Three-way test, client crashes and server detects\n";

$MD = $middle->CreateProcess ("middle",
                              "-o $middle_midiorfile ".
                              "-k file://$middle_iorfile");

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
if ($middle->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$middle_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$middle_status = $MD->Spawn ();

if ($middle_status != 0) {
    print STDERR "ERROR: server returned $middle_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($middle->WaitForFileTimed ($midiorbase,
                               $middle->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$middle_midiorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    $MD->Kill (); $MD->TimedWait (1);
    exit 1;
}

if ($middle->GetFile ($midiorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$middle_midiorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    $MD->Kill (); $MD->TimedWait (1);
    exit 1;
}
if ($client->PutFile ($midiorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_midiorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    $MD->Kill (); $MD->TimedWait (1);
    exit 1;
}

$CL->Arguments ("-k file://$client_midiorfile -i 100 -s");

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 85);

if ($client_status == -1) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    $MD->Kill (); $MD->TimedWait (1);
    exit 1;
}

$CL->Arguments ("-k file://$client_midiorfile -i 10 -x");

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 85);

if ($client_status == -1) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    $MD->Kill (); $MD->TimedWait (1);
    exit 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    $MD->Kill (); $MD->TimedWait (1);
    exit 1;
}

$middle_status = $MD->WaitKill ($middle->ProcessStopWaitInterval());

if ($middle_status != 0) {
    print STDERR "ERROR: server returned $middle_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($midiorbase);
$middle->DeleteFile($iorbase);
$middle->DeleteFile($midiorbase);

########################################################################
########################################################################
########################################################################
print STDERR "################ Thread-Per-Connection Config###############\n";
print STDERR "===== Base test, no crashes\n";

$SV->Arguments ("-ORBdebuglevel $debug_level ".
                "-ORBSvcConf $server_svcfile -o $server_iorfile");
$CL->Arguments ("-k file://$client_iorfile -i 100");

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
if ($client->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

###############################################################################
print STDERR "===== Client crash (abort) during upcall\n";

$CL->Arguments ("-k file://$client_iorfile -i 100 -s");

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status == -1) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

###############################################################################
print STDERR "===== Client crash during upcall\n";

$CL->Arguments ("-k file://$client_iorfile -i 100 -z");

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status == -1) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

###############################################################################
print STDERR "===== Server crash (abort) during upcall\n";

$CL->Arguments ("-k file://$client_iorfile -i 100 -a");

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status == -1) {
    print STDERR "ERROR: server returned $server_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

###############################################################################
print STDERR "===== Server crash during upcall\n";

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
if ($client->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$CL->Arguments ("-k file://$client_iorfile -i 100 -c");

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status == -1) {
    print STDERR "ERROR: server returned $server_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

###############################################################################
print STDERR "===== Three-way test, client crashes and server detects\n";

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
if ($middle->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$middle_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$middle_status = $MD->Spawn ();

if ($middle_status != 0) {
    print STDERR "ERROR: server returned $middle_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($middle->WaitForFileTimed ($midiorbase,
                               $middle->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$middle_midiorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    $MD->Kill (); $MD->TimedWait (1);
    exit 1;
}

if ($middle->GetFile ($midiorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$middle_midiorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    $MD->Kill (); $MD->TimedWait (1);
    exit 1;
}
if ($client->PutFile ($midiorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_midiorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    $MD->Kill (); $MD->TimedWait (1);
    exit 1;
}

$CL->Arguments ("-k file://$client_midiorfile -i 100 -s");

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 85);

if ($client_status == -1) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    $MD->Kill (); $MD->TimedWait (1);
    exit 1;
}

$CL->Arguments ("-k file://$client_midiorfile -i 10 -x");

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 85);

if ($client_status == -1) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    $MD->Kill (); $MD->TimedWait (1);
    exit 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    $MD->Kill (); $MD->TimedWait (1);
    exit 1;
}

$middle_status = $MD->WaitKill ($middle->ProcessStopWaitInterval());

if ($middle_status != 0) {
    print STDERR "ERROR: server returned $middle_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($midiorbase);
$middle->DeleteFile($iorbase);
$middle->DeleteFile($midiorbase);

exit $status;
