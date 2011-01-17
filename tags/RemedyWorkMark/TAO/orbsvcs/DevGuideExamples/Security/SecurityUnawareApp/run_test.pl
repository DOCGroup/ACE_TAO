# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $srv = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $cli = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $cli1 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $cli2 = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";

$srviorfile = "Messenger.ior";
$srvconf = "server.conf";
$cliconf = "client.conf";
$cli1conf = "client1.conf";

my $srv_srviorfile = $srv->LocalFile ($srviorfile);
my $cli_srviorfile = $cli->LocalFile ($srviorfile);
my $cli1_srviorfile = $cli1->LocalFile ($srviorfile);
my $cli2_srviorfile = $cli2->LocalFile ($srviorfile);
my $srv_srvconf = $srv->LocalFile ($srvconf);
my $cli_cliconf = $srv->LocalFile ($cliconf);
my $cli_cli1conf = $srv->LocalFile ($cli1conf);
$srv->DeleteFile ($srviorfile);
$cli->DeleteFile ($srviorfile);
$cli1->DeleteFile ($srviorfile);
$cli2->DeleteFile ($srviorfile);

$ENV{'SSL_CERT_FILE'} = 'cacert.pem';

$SRV = $srv->CreateProcess ("MessengerServer", "-ORBdebuglevel $debug_level ".
                                               "-o $srv_srviorfile ".
                                               "-ORBSvcConf $srv_srvconf");
$CLI = $cli->CreateProcess ("MessengerClient", "-k file://$cli_srviorfile ");
$CLI1 = $cli1->CreateProcess ("MessengerClient", "-k file://$cli_srviorfile ".
                                               "-ORBSvcConf $cli_cliconf");
$CLI2 = $cli2->CreateProcess ("MessengerClient", "-k file://$cli1_srviorfile ".
                                               "-ORBSvcConf $cli_cli1conf");

# start MessengerServer
print STDERR "\n\nSecurity Unaware Application Examples\n";
print STDERR     "-------------------------------------\n";
print STDERR     "Starting Messenger Server...\n\n";

$SRV_status = $SRV->Spawn ();

if ($SRV_status != 0) {
    print STDERR "ERROR: Server returned $SRV_status\n";
    exit 1;
}

if ($srv->WaitForFileTimed ($srviorfile,$srv->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$srv_srviorfile>\n";
    $SRV->Kill (); $SRV->TimedWait (1);
    exit 1;
}

if ($srv->GetFile ($srviorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$srv_srviorfile>\n";
    $SRV->Kill (); $SRV->TimedWait (1);
    exit 1;
}
if ($cli->PutFile ($srviorfile) == -1) {
    print STDERR "ERROR: cannot set file <$cli_srviorfile>\n";
    $SRV->Kill (); $SRV->TimedWait (1);
    exit 1;
}
if ($cli1->PutFile ($srviorfile) == -1) {
    print STDERR "ERROR: cannot set file <$cli1_srviorfile>\n";
    $SRV->Kill (); $SRV->TimedWait (1);
    exit 1;
}
if ($cli2->PutFile ($srviorfile) == -1) {
    print STDERR "ERROR: cannot set file <$cli2_srviorfile>\n";
    $SRV->Kill (); $SRV->TimedWait (1);
    exit 1;
}

# start MessengerClient

print STDERR     "\nStarting MessengerClient, example 1...\n\n";
$CLI_status = $CLI->SpawnWaitKill ($cli->ProcessStartWaitInterval());

if ($CLI_status == 0) {
    print STDERR "ERROR: Messenger Client returned $CLI_status\n";
    $SRV->Kill (); $SRV->TimedWait (1);
    $status = 1;
}

print STDERR     "\nStarting MessengerClient, example 2...\n\n";
$CLI1_status = $CLI1->SpawnWaitKill ($cli1->ProcessStartWaitInterval());

if ($CLI1_status != 0) {
    print STDERR "ERROR: Messenger Client1 returned $CLI1_status\n";
    $SRV->Kill (); $SRV->TimedWait (1);
    $status = 1;
}

$ENV{'SSL_CERT_FILE'} = '';

print STDERR     "\nStarting MessengerClient, example 3...\n\n";
$CLI2_status = $CLI2->SpawnWaitKill ($cli2->ProcessStartWaitInterval());

if ($CLI2_status != 0) {
    print STDERR "ERROR: Messenger Client2 returned $CLI2_status\n";
    $SRV->Kill (); $SRV->TimedWait (1);
    $status = 1;
}

# clean-up
$SRV_status = $SRV->TerminateWaitKill ($srv->ProcessStopWaitInterval());

if ($SRV_status != 0) {
    print STDERR "ERROR: Messenger Server returned $SRV_status\n";
    exit 1;
}

$srv->DeleteFile ($srviorfile);
$cli->DeleteFile ($srviorfile);
$cli1->DeleteFile ($srviorfile);
$cli2->DeleteFile ($srviorfile);

exit $status;
