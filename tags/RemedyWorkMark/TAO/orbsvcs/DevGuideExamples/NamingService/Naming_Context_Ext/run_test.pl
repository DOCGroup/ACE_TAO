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

my $ns = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $srv = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $cli = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

my $nsiorfile = "ns.ior";
my $srviorfile = "url.ior";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $srv_srviorfile = $srv->LocalFile ($srviorfile);
my $cli_srviorfile = $cli->LocalFile ($srviorfile);
$ns->DeleteFile ($nsiorfile);
$srv->DeleteFile ($srviorfile);
$cli->DeleteFile ($srviorfile);

$hostname = $ns->HostName ();
$port = "2809";

$NameService = "$ENV{TAO_ROOT}/orbsvcs/Naming_Service/tao_cosnaming";
$NS = $ns->CreateProcess ($NameService, "-ORBListenEndpoints iiop://$hostname:$port ".
                                        " -o $ns_nsiorfile");
$SRV = $srv->CreateProcess ("MessengerServer", "-ORBdebuglevel $debug_level ".
                                               "-o $srv_srviorfile ".
                                               "-h $hostname ".
                                               "-p $port ".
                                               "-ORBInitRef NameService=iiop://$hostname:$port/NameService");
$CLI = $cli->CreateProcess ("MessengerClient", "-k file://$cli_srviorfile ".
                                               "-ORBInitRef NameService=iiop://$hostname:$port/NameService");

# start Naming Service
$NS_status = $NS->Spawn ();

if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    exit 1;
}

if ($ns->WaitForFileTimed ($nsiorfile,$ns->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ns_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

# start MessengerServer
$SRV_status = $SRV->Spawn ();

if ($SRV_status != 0) {
    print STDERR "ERROR: Messenger Server returned $SRV_status\n";
    exit 1;
}

if ($srv->WaitForFileTimed ($srviorfile,$srv->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$srv_srviorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    $SRV->Kill (); $SRV->TimedWait (1);
    exit 1;
}

if ($srv->GetFile ($srviorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$srv_srviorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    $SRV->Kill (); $SRV->TimedWait (1);
    exit 1;
}
if ($cli->PutFile ($srviorfile) == -1) {
    print STDERR "ERROR: cannot set file <$cli_srviorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    $SRV->Kill (); $SRV->TimedWait (1);
    exit 1;
}

# start MessengerClient
$CLI_status = $CLI->SpawnWaitKill ($cli->ProcessStartWaitInterval());

if ($CLI_status != 0) {
    print STDERR "ERROR: Messenger Client failed with status $CLI_status\n";
    $status = 1;
}

# clean-up
$SRV_status = $SRV->TerminateWaitKill ($srv->ProcessStopWaitInterval());

if ($SRV_status != 0) {
    print STDERR "ERROR: Messenger Server returned $SRV_status\n";
    $status = 1;
}

$NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval());

if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    $status = 1;
}

$ns->DeleteFile ($nsiorfile);
$srv->DeleteFile ($srviorfile);
$cli->DeleteFile ($srviorfile);

exit $status;
