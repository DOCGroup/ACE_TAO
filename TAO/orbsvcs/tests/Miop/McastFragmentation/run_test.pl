eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$server_level = '0';
$client_level = '0';

$orbs = 10;
$payload = 3000;
$threads = 10;
$count = 10;

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $server_level = '3';
        # Level enough for reporting errors.
        $client_level = '9';
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client1 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $client2 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $client3 = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";

$uipmc = "corbaloc:miop:1.0\@1.0-domain-1/225.10.10.10:" . $server->RandomPort();

my $c_svcconf = "uipmc_client$PerlACE::svcconf_ext";
my $s_svcconf = "uipmc_server$PerlACE::svcconf_ext";
my $cleanup_select = 0; #int rand (3);
if ($cleanup_select == 1) {
    $s_svcconf = "uipmc_server_n$PerlACE::svcconf_ext";
} elsif ($cleanup_select == 2) {
    $s_svcconf = "uipmc_server_m$PerlACE::svcconf_ext";
}
print STDOUT "Using $s_svcconf for the server initialization\n";

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $server_svcconf = $server->LocalFile ($s_svcconf);
my $client1_iorfile = $client1->LocalFile ($iorbase);
my $client1_svcconf = $client1->LocalFile ($c_svcconf);
my $client2_iorfile = $client2->LocalFile ($iorbase);
my $client2_svcconf = $client2->LocalFile ($c_svcconf);
my $client3_iorfile = $client3->LocalFile ($iorbase);
my $client3_svcconf = $client3->LocalFile ($c_svcconf);
$server->DeleteFile($iorbase);
$client1->DeleteFile($iorbase);
$client2->DeleteFile($iorbase);
$client3->DeleteFile($iorbase);

$SV = $server->CreateProcess ("server",
                              "-ORBdebuglevel $server_level " .
                              "-ORBSvcConf $server_svcconf " .
                              "-ORBRcvSock 500000 " .
                              "-o $server_iorfile -u $uipmc -s $orbs " .
                              "-p $payload -t " .
                              $threads * 1 . " -c $count");
$CL1 = $client1->CreateProcess ("client",
                                "-ORBSvcConf $client1_svcconf " .
                                "-ORBDebugLevel $client_level " .
                                "-k file://$client1_iorfile -p $payload " .
                                "-t $threads -c $count -f 0");
=COMMENT
$CL2 = $client2->CreateProcess ("client",
                                "-ORBSvcConf $client2_svcconf " .
                                "-k file://$client2_iorfile -p $payload " .
                                "-t $threads -c $count -f $threads");
$CL3 = $client3->CreateProcess ("client",
                                "-ORBSvcConf $client3_svcconf " .
                                "-k file://$client3_iorfile -p $payload " .
                                "-t $threads -c $count " .
                                "-f " . $threads * 2);
=cut

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

$client_status = $CL1->Spawn ();
if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

=COMMENT
$client_status = $CL2->Spawn ();
if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    $CL1->Kill (); $CL1->TimedWait (1);
    exit 1;
}

$client_status = $CL3->Spawn ();
if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    $CL1->Kill (); $CL1->TimedWait (1);
    $CL2->Kill (); $CL2->TimedWait (1);
    exit 1;
}
=cut

$client_status = $CL1->WaitKill ($client1->ProcessStopWaitInterval() +
                                 $count * $threads);
if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    # $CL2->Kill (); $CL2->TimedWait (1);
    # $CL3->Kill (); $CL3->TimedWait (1);
    exit 1;
}

=COMMENT
$client_status = $CL2->WaitKill ($client2->ProcessStopWaitInterval() +
                                 $count * $threads);
if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    $CL3->Kill (); $CL3->TimedWait (1);
    exit 1;
}

$client_status = $CL3->WaitKill ($client3->ProcessStopWaitInterval() +
                                 $count * $threads);
if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
=cut

# Shutdown the server.
$CL1->Arguments ("-ORBSvcConf $client1_svcconf -k file://$client1_iorfile -x");
$client_status = $CL1->Spawn ();
if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client_status = $CL1->WaitKill ($client1->ProcessStopWaitInterval());
if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
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

exit $status;
