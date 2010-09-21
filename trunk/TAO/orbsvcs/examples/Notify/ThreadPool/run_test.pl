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
my $nfs = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $sup = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $con = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $con2 = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";

PerlACE::check_privilege_group();

# ThreadPool Example
#

$experiment_timeout = 60;
$startup_timeout = 60;
$max_events = 10;
$ec_tp = 0;
$pc_tp = 0;
$ps1_tp = 1;
$ps2_tp = 1;
$debug = 0;

$nsiorfile = "naming.ior";
$nfsiorfile = "notify.ior";
$supiorfile = "supplier.ior";
$nfsconf = "notify.conf";
$cliconf = "client.conf";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $nfs_nsiorfile = $nfs->LocalFile ($nsiorfile);
my $sup_nsiorfile = $sup->LocalFile ($nsiorfile);
my $con_nsiorfile = $con->LocalFile ($nsiorfile);
my $con2_nsiorfile = $con2->LocalFile ($nsiorfile);
my $nfs_nfsiorfile = $nfs->LocalFile ($nfsiorfile);
my $sup_supiorfile = $sup->LocalFile ($supiorfile);
my $nfs_nfsconf = $nfs->LocalFile ($nfsconf);
my $sup_cliconf = $sup->LocalFile ($cliconf);
my $con_cliconf = $con->LocalFile ($cliconf);
my $con2_cliconf = $con2->LocalFile ($cliconf);
$ns->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nsiorfile);
$sup->DeleteFile ($nsiorfile);
$con->DeleteFile ($nsiorfile);
$con2->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nfsiorfile);
$sup->DeleteFile ($supiorfile);


$NS = $ns->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Naming_Service/Naming_Service",
                          " -o $ns_nsiorfile");

$NFS = $nfs->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Notify_Service/tao_cosnotification");
$Notify_Args = "-ORBInitRef NameService=file://$nfs_nsiorfile -IORoutput $nfs_nfsiorfile -ORBSvcConf $nfs_nfsconf -ORBDebugLevel $debug";

$SUP = $sup->CreateProcess ("Supplier");
$Supplier_Args = "-ORBInitRef NameService=file://$sup_nsiorfile -ORBSvcConf $sup_cliconf -IORoutput $sup_supiorfile -Consumers 2 -EventChannel_ThreadPool $ec_tp -MaxEvents $max_events -ProxyConsumer_ThreadPool $pc_tp -ORBDebugLevel $debug";

$CON = $sup->CreateProcess ("Consumer");
$Consumer_Args = "-ORBInitRef NameService=file://$con_nsiorfile -ORBSvcConf $con_cliconf -MaxEvents $max_events -Delay 1 -ProxySupplier_ThreadPool $ps1_tp -ORBDebugLevel $debug";

$CON2 = $sup->CreateProcess ("Consumer");
$Consumer2_Args = "-ORBInitRef NameService=file://$con2_nsiorfile -ORBSvcConf $con2_cliconf -MaxEvents $max_events -ProxySupplier_ThreadPool $ps2_tp -ORBDebugLevel $debug";

$NS_status = $NS->Spawn ();

if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
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
if ($nfs->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$nfs_nsiorfile>\n";
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
if ($con2->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$con2_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$NFS->Arguments ($Notify_Args);
$args = $NFS->Arguments ();
print STDERR "Running Notification with arguments: $args\n";
$NFS_status = $NFS->Spawn ();

if ($NFS_status != 0) {
    print STDERR "ERROR: Notification Service returned $NFS_status\n";
    exit 1;
}

if ($nfs->WaitForFileTimed ($nfsiorfile,$nfs->ProcessStartWaitInterval()+$startup_timeout) == -1) {
    print STDERR "ERROR: cannot find file <$nfs_nfsiorfile>\n";
    $NFS->Kill (); $NFS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$SUP->Arguments ($Supplier_Args);
$args = $SUP->Arguments ();
print STDERR "Running Supplier with arguments: $args\n";
$SUP_status = $SUP->Spawn ();

if ($SUP_status != 0) {
    print STDERR "ERROR: Supplier returned $SUP_status\n";
    exit 1;
}

if ($sup->WaitForFileTimed ($supiorfile,$sup->ProcessStartWaitInterval()+$startup_timeout) == -1) {
    print STDERR "ERROR: cannot find file <$sup_supiorfile>\n";
    $NFS->Kill (); $NFS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$CON->Arguments ($Consumer_Args);
$args = $CON->Arguments ();
print STDERR "Running Consumer with arguments: $args\n";
$CON_status = $CON->Spawn ();

if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$CON2->Arguments ($Consumer2_Args);
$args = $CON2->Arguments ();
print STDERR "Running Consumer2 with arguments: $args\n";
$CON2_status = $CON2->SpawnWaitKill ($con2->ProcessStartWaitInterval()+$experiment_timeout);
if ($CON2_status != 0) {
    print STDERR "ERROR: Consumer2 returned $CON2_status\n";
    $status = 1;
}

$CON->Wait ();
$SUP->Wait ();

$NFS_status = $NFS->TerminateWaitKill ($nfs->ProcessStopWaitInterval());

if ($NFS_status != 0) {
    print STDERR "ERROR: Notification Service returned $NFS_status\n";
    $status = 1;
}

$NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval());

if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    $status = 1;
}

$ns->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nsiorfile);
$sup->DeleteFile ($nsiorfile);
$con->DeleteFile ($nsiorfile);
$con2->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nfsiorfile);
$sup->DeleteFile ($supiorfile);

exit $status;
