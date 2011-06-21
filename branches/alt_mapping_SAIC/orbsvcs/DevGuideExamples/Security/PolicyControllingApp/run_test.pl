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
my $srv1 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $cli1 = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";

$srviorfile = "Messenger.ior";
$srvconf = "server.conf";
$srv1conf = "server1.conf";
$cliconf = "client.conf";
$cli1conf = "client1.conf";

my $srv_srviorfile = $srv->LocalFile ($srviorfile);
my $cli_srviorfile = $cli->LocalFile ($srviorfile);
my $srv1_srviorfile = $srv1->LocalFile ($srviorfile);
my $cli1_srviorfile = $cli1->LocalFile ($srviorfile);
my $srv_srvconf = $srv->LocalFile ($srvconf);
my $srv_srv1conf = $srv->LocalFile ($srv1conf);
my $cli_cliconf = $srv->LocalFile ($cliconf);
my $cli_cli1conf = $srv->LocalFile ($cli1conf);
$srv->DeleteFile ($srviorfile);
$cli->DeleteFile ($srviorfile);
$srv1->DeleteFile ($srviorfile);
$cli1->DeleteFile ($srviorfile);

$ENV{'SSL_CERT_FILE'} = 'cacert.pem';

$SRV = $srv->CreateProcess ("MessengerServer", "-ORBdebuglevel $debug_level ".
                                               "-o $srv_srviorfile ".
                                               "-ORBSvcConf $srv_srvconf");
$SRV1 = $srv1->CreateProcess ("MessengerServer", "-ORBdebuglevel $debug_level ".
                                               "-o $srv1_srviorfile ".
                                               "-ORBSvcConf $srv_srv1conf");
$CLI = $cli->CreateProcess ("MessengerClient", "-e 1 ".
                                               "-k file://$cli_srviorfile ".
                                               "-ORBSvcConf $cli_cliconf");
$CLI1 = $cli1->CreateProcess ("MessengerClient", "-e 2 ".
                                               "-k file://$cli1_srviorfile ".
                                               "-ORBSvcConf $cli_cli1conf");


# start MessengerServer
print STDERR "\n\nSecurity Policy Controlling Application Examples\n";
print STDERR     "------------------------------------------------\n";

print STDERR     "Starting Messenger Server, example 1...\n\n";
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

print STDERR     "\nStarting MessengerClient, example 1...\n\n";
$CLI_status = $CLI->SpawnWaitKill ($cli->ProcessStartWaitInterval());

if ($CLI_status != 0) {
    print STDERR "ERROR: Messenger Client returned $CLI_status\n";
    $SRV->Kill (); $SRV->TimedWait (1);
    exit 1;
}

$SRV_status = $SRV->TerminateWaitKill ($srv->ProcessStopWaitInterval());

if ($SRV_status != 0) {
    print STDERR "ERROR: Messenger Server returned $SRV_status\n";
    exit 1;
}

$srv->DeleteFile ($srviorfile);
$cli->DeleteFile ($srviorfile);


print STDERR     "\nStarting Messenger Server, example 2...\n\n";
$SRV_status = $SRV1->Spawn ();

if ($SRV_status != 0) {
    print STDERR "ERROR: Server returned $SRV_status\n";
    exit 1;
}

if ($srv1->WaitForFileTimed ($srviorfile,$srv1->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$srv1_srviorfile>\n";
    $SRV1->Kill (); $SRV1->TimedWait (1);
    exit 1;
}

if ($srv1->GetFile ($srviorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$srv1_srviorfile>\n";
    $SRV1->Kill (); $SRV1->TimedWait (1);
    exit 1;
}
if ($cli1->PutFile ($srviorfile) == -1) {
    print STDERR "ERROR: cannot set file <$cli1_srviorfile>\n";
    $SRV1->Kill (); $SRV1->TimedWait (1);
    exit 1;
}

print STDERR     "\nStarting MessengerClient, example 2...\n\n";
$CLI_status = $CLI1->SpawnWaitKill ($cli1->ProcessStartWaitInterval());

if ($CLI_status != 0) {
    print STDERR "ERROR: Messenger Client returned $CLI_status\n";
    $SRV1->Kill (); $SRV1->TimedWait (1);
    $status = 1;
}

$SRV_status = $SRV1->TerminateWaitKill ($srv1->ProcessStopWaitInterval());

if ($SRV_status != 0) {
    print STDERR "ERROR: Messenger Server returned $SRV_status\n";
    $status = 1;
}

$srv1->DeleteFile ($srviorfile);
$cli1->DeleteFile ($srviorfile);

exit $status;
