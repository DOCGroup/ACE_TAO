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

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $srv_nsiorfile = $srv->LocalFile ($nsiorfile);
my $cli_nsiorfile = $cli->LocalFile ($nsiorfile);
$ns->DeleteFile ($nsiorfile);
$srv->DeleteFile ($nsiorfile);
$cli->DeleteFile ($nsiorfile);

# start Naming Service
$NameService = "$ENV{TAO_ROOT}/orbsvcs/Naming_Service/Naming_Service";
$NS = $ns->CreateProcess ($NameService, " -o $ns_nsiorfile");
$SRV = $srv->CreateProcess ("MessengerServer", "-ORBdebuglevel $debug_level ".
                                               "-ORBInitRef NameService=file://$srv_nsiorfile");
$CLI = $cli->CreateProcess ("MessengerClient", "-ORBInitRef NameService=file://$cli_nsiorfile");

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

if ($ns->GetFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ns_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($srv->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$srv_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($cli->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$cli_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

# start MessengerServer
$SRV_status = $SRV->Spawn ();

if ($SRV_status != 0) {
    print STDERR "ERROR: Messenger Server returned $SRV_status\n";
    exit 1;
}

# start MessengerClient
$CLI_status = $CLI->SpawnWaitKill ($cli->ProcessStartWaitInterval());

if ($CLI_status != 0) {
    print STDERR "ERROR: Messenger Client returned $CLI_status\n";
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
$srv->DeleteFile ($nsiorfile);
$cli->DeleteFile ($nsiorfile);

exit $status;
