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

my $ns = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $ls = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $cli = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

$nsiorfile = "ns.ior";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $ls_nsiorfile = $ls->LocalFile ($nsiorfile);
my $cli_nsiorfile = $cli->LocalFile ($nsiorfile);
$ns->DeleteFile ($nsiorfile);
$ls->DeleteFile ($nsiorfile);
$cli->DeleteFile ($nsiorfile);


$NS = $ns->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Naming_Service/Naming_Service",
                          " -o $ns_nsiorfile");
$LS = $ls->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Logging_Service/Basic_Logging_Service/Basic_Logging_Service",
                          " -ORBInitRef NameService=file://$ls_nsiorfile");
$CLI = $cli->CreateProcess ("client",
                            "-ORBInitRef NameService=file://$cli_nsiorfile");

print STDERR "Starting Naming Service\n";

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
if ($ls->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$ls_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($cli->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$cli_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

print STDERR "Starting Logging Service\n";

$LS_status = $LS->Spawn ();

if ($LS_status != 0) {
    print STDERR "ERROR: LOgging Service returned $LS_status\n";
    $LS->Kill (); $LS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

# Give time for logging service to initialize and install its object
# reference in the nameing service.
sleep (5);

print STDERR "Starting client\n";

$CLI_status = $CLI->Spawn ();
if ($CLI_status != 0) {
    print STDERR "ERROR: Client returned $CLI_status\n";
    $status = 1;
}

$CLI_status = $CLI->WaitKill ($cli->ProcessStopWaitInterval());
if ($CLI_status != 0) {
    print STDERR "ERROR: Client returned $CLI_status\n";
    $status = 1;
}

$LS_status = $LS->TerminateWaitKill ($ls->ProcessStopWaitInterval());

if ($LS_status != 0) {
    print STDERR "ERROR: Logging Service returned $LS_status\n";
    $status = 1;
}

$NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval());

if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    $status = 1;
}

$ns->DeleteFile ($nsiorfile);
$ls->DeleteFile ($nsiorfile);
$cli->DeleteFile ($nsiorfile);

exit $status;
