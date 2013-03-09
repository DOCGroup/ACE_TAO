eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use Getopt::Std;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $ns = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $sup = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $con = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $rel = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";

$experiment_timeout = 60;
$startup_timeout = 60;

$nsiorfile = "naming.ior";
$reliorfile = "relay.ior";
$coniorfile = "consumer.ior";
$supconffile = "supplier.conf";
$conconffile = "consumer.conf";
$relconffile = "relay.conf";

$chighdat = "c_high.dat";
$shighdat = "s_high.dat";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $rel_nsiorfile = $rel->LocalFile ($nsiorfile);
my $sup_nsiorfile = $sup->LocalFile ($nsiorfile);
my $con_nsiorfile = $con->LocalFile ($nsiorfile);
my $rel_reliorfile = $rel->LocalFile ($reliorfile);
my $sup_reliorfile = $sup->LocalFile ($reliorfile);
my $con_coniorfile = $con->LocalFile ($coniorfile);
my $rel_coniorfile = $rel->LocalFile ($coniorfile);
my $sup_supconffile = $sup->LocalFile ($supconffile);
my $con_conconffile = $con->LocalFile ($conconffile);
my $rel_relconffile = $rel->LocalFile ($relconffile);
my $con_chighdat = $con->LocalFile ($chighdat);
my $sup_shighdat = $sup->LocalFile ($shighdat);
$ns->DeleteFile ($nsiorfile);
$rel->DeleteFile ($nsiorfile);
$sup->DeleteFile ($nsiorfile);
$con->DeleteFile ($nsiorfile);
$con->DeleteFile ($coniorfile);
$rel->DeleteFile ($coniorfile);
$rel->DeleteFile ($reliorfile);
$sup->DeleteFile ($reliorfile);
$con->DeleteFile ($chighdat);
$sup->DeleteFile ($shighdat);

$NS = $ns->CreateProcess ("../../../../../../Naming_Service/tao_cosnaming",
                          "-o $ns_nsiorfile");

$SUP = $sup->CreateProcess ("../../../../Driver/Notify_Tests_Driver");
$SUP_Args = "-ORBDebugLevel $debug_level ".
            "-ORBInitRef NameService=file://$sup_nsiorfile ".
            "-IORinput file://$sup_reliorfile ".
            "-ORBSvcConf $sup_supconffile ".
            "-Skip_Priority_Levels_Check";

$CON = $con->CreateProcess ("../../../../Driver/Notify_Tests_Driver");
$CON_Args = "-ORBInitRef NameService=file://$con_nsiorfile ".
            "-IORoutput $con_coniorfile ".
            "-ORBSvcConf $con_conconffile ".
            "-Skip_Priority_Levels_Check";

$REL = $rel->CreateProcess ("../../../../Driver/Notify_Tests_Driver");
$REL_Args = "-ORBInitRef NameService=file://$rel_nsiorfile ".
            "-IORoutput $rel_reliorfile ".
            "-IORinput file://$rel_coniorfile ".
            "-ORBSvcConf $rel_relconffile ".
            "-Skip_Priority_Levels_Check";

$NS_status = $NS->Spawn ();
if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($ns->WaitForFileTimed ($nsiorfile,$ns->ProcessStartWaitInterval()+$startup_timeout) == -1) {
    print STDERR "ERROR: cannot find file <$ns_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($ns->GetFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ns_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($rel->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$rel_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($sup->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$sup_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($con->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$con_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$CON->Arguments ($CON_Args);
$args = $CON->Arguments ();
print STDERR "Running Consumer with arguments: $args\n";
$CON_status = $CON->Spawn ();
if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($con->WaitForFileTimed ($coniorfile,$con->ProcessStartWaitInterval()+$startup_timeout) == -1) {
    print STDERR "ERROR: cannot find file <$con_coniorfile>\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($con->GetFile ($coniorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$con_coniorfile>\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($rel->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$rel_nsiorfile>\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$REL->Arguments ($REL_Args);
$args = $REL->Arguments ();
print STDERR "Running Relay with arguments: $args\n";
$REL_status = $REL->Spawn ();
if ($REL_status != 0) {
    print STDERR "ERROR: Relay returned $REL_status\n";
    $REL->Kill (); $REL->TimedWait (1);
    $CON->Kill (); $CON->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($rel->WaitForFileTimed ($reliorfile,$rel->ProcessStartWaitInterval()+$startup_timeout) == -1) {
    print STDERR "ERROR: cannot find file <$rel_reliorfile>\n";
    $REL->Kill (); $REL->TimedWait (1);
    $CON->Kill (); $CON->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$SUP->Arguments ($SUP_Args);
$args = $SUP->Arguments ();
print STDERR "Running Supplier with arguments: $args\n";
$SUP_status = $SUP->SpawnWaitKill ($sup->ProcessStartWaitInterval()+$experiment_timeout);
if ($SUP_status != 0) {
    print STDERR "ERROR: Supplier returned $SUP_status\n";
    $status = 1;
}

$SUP_status = $SUP->TerminateWaitKill ($sup->ProcessStopWaitInterval());
if ($SUP_status != 0) {
    print STDERR "ERROR: Closing Supplier returned $SUP_status\n";
    $status = 1;
}

$CON_status = $CON->TerminateWaitKill ($con->ProcessStopWaitInterval());
if ($CON_status != 0) {
    print STDERR "ERROR: Closing Consumer returned $CON_status\n";
    $status = 1;
}
$REL_status = $REL->TerminateWaitKill ($rel->ProcessStopWaitInterval());
if ($REL_status != 0) {
    print STDERR "ERROR: Closing Relay returned $REL_status\n";
    $status = 1;
}

$NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval());
if ($NS_status != 0) {
    print STDERR "ERROR: Closing Name Service returned $NS_status\n";
    $status = 1;
}

if ($#ARGV > -1) {
    print STDERR "Results are not saved in $ARGV[0]\n";
}

$ns->DeleteFile ($nsiorfile);
$rel->DeleteFile ($nsiorfile);
$sup->DeleteFile ($nsiorfile);
$con->DeleteFile ($nsiorfile);
$con->DeleteFile ($coniorfile);
$rel->DeleteFile ($coniorfile);
$rel->DeleteFile ($reliorfile);
$sup->DeleteFile ($reliorfile);
$con->DeleteFile ($chighdat);
$sup->DeleteFile ($shighdat);

exit $status;
