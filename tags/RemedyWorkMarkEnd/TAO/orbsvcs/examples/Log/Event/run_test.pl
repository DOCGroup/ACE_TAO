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
my $con = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $sup = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";

$nsiorfile = "ns.ior";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $ls_nsiorfile = $ls->LocalFile ($nsiorfile);
my $con_nsiorfile = $con->LocalFile ($nsiorfile);
my $sup_nsiorfile = $sup->LocalFile ($nsiorfile);
$ns->DeleteFile ($nsiorfile);
$ls->DeleteFile ($nsiorfile);
$con->DeleteFile ($nsiorfile);
$sup->DeleteFile ($nsiorfile);

$NS = $ns->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Naming_Service/tao_cosnaming",
                          " -o $ns_nsiorfile");
$LS = $ls->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Logging_Service/Event_Logging_Service/tao_tls_event",
                          " -ORBInitRef NameService=file://$ls_nsiorfile");
$CON = $con->CreateProcess ("Event_Consumer",
                            "-ORBInitRef NameService=file://$con_nsiorfile");
$SUP = $sup->CreateProcess ("Event_Supplier",
                            "-ORBInitRef NameService=file://$sup_nsiorfile");

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
if ($con->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$con_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($sup->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$sup_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

print STDERR "Starting Logging Service\n";

$LS_status = $LS->Spawn ();

if ($LS_status != 0) {
    print STDERR "ERROR: Logging Service returned $LS_status\n";
    $LS->Kill (); $LS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

# Give time for logging service to initialize and install its object
# reference in the naming service.
sleep (5);

print STDERR "Starting Consumer\n";

$CON_status = $CON->Spawn ();
if ($CON_status != 0) {
    print STDERR "ERROR: Event Consumer returned $CON_status\n";
    $CON->Kill (); $CON->TimedWait (1);
    $LS->Kill (); $LS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

sleep (1);

print STDERR "Starting Supplier\n";

$SUP_status = $SUP->Spawn ();
if ($SUP_status != 0) {
    print STDERR "ERROR: Event Supplier returned $SUP_status\n";
    $status = 1;
}

$SUP_status = $SUP->WaitKill ($sup->ProcessStopWaitInterval());
if ($SUP_status != 0) {
    print STDERR "ERROR: Event Supplier returned $SUP_status\n";
    $status = 1;
}

$CON_status = $CON->TerminateWaitKill ($con->ProcessStopWaitInterval());

if ($CON_status != 0) {
    print STDERR "ERROR: Event Consumer returned $CON_status\n";
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
$con->DeleteFile ($nsiorfile);
$sup->DeleteFile ($nsiorfile);

exit $status;
