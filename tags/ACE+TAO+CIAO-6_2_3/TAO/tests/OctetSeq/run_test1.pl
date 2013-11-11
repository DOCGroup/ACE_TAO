eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

$client_iterations = '5000';

$octet_iterations = '32';
$low = '8192';
$high = '8192';
$step = '1';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $t3 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

$status = 0;

my $iorbase = "test.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $t3->LocalFile ($iorbase);

my $conf_base = "svc1" . $PerlACE::svcconf_ext;
my $server_conf = $server->LocalFile ($conf_base);
my $client_conf = $client->LocalFile ($conf_base);
my $t3_conf = $t3->LocalFile ($conf_base);

if ($server->PutFile ($conf_base) == -1) {
    print STDERR "ERROR: cannot set file <$server_conf>\n";
    exit 1;
}

if ($client->PutFile ($conf_base) == -1) {
    print STDERR "ERROR: cannot set file <$client_conf>\n";
    exit 1;
}

if ($t3->PutFile ($conf_base) == -1) {
    print STDERR "ERROR: cannot set file <$t3_conf>\n";
    exit 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$SV = $server->CreateProcess ("server",
                              "-ORBdebuglevel $debug_level " .
                              "-ORBSvcConf $server_conf " .
                              "-o $server_iorfile");

$T = $client->CreateProcess ("OctetSeq",
                             "-ORBdebuglevel $debug_level " .
                             "-ORBSvcConf $client_conf " .
                             "-n $octet_iterations -l $low -h $high -s $step -q");

$CL = $t3->CreateProcess ("client",
                          "-ORBSvcConf $t3_conf " .
                          "-i $client_iterations -k file://$client_iorfile");

print STDERR "\n\n==== Octet sequence passing test\n";

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

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 100);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval() + 10);

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

print STDERR "\n\n==== Octet sequence performance test\n";

$test = $T->SpawnWaitKill ($t3->ProcessStartWaitInterval() + 45);

if ($test != 0) {
    print STDERR "ERROR: test returned $test\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
