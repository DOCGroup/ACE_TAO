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

$experiment_timeout = 60;
$startup_timeout = 60;

$nfsiorfile = "notify.ior";
$nsiorfile = "naming.ior";
$svc_conf = "NS.conf";
$persistent_xml = "persistency.notif.xml";
$persistent_000 = "persistency.notif.000";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $nfs_nsiorfile = $nfs->LocalFile ($nsiorfile);
my $sup_nsiorfile = $sup->LocalFile ($nsiorfile);
my $con_nsiorfile = $con->LocalFile ($nsiorfile);
my $nfs_nfsiorfile = $nfs->LocalFile ($nfsiorfile);
my $nfs_svc_conf = $nfs->LocalFile ($svc_conf);
my $nfs_persistent_xml = $nfs->LocalFile ($persistent_xml);
my $nfs_persistent_000 = $nfs->LocalFile ($persistent_000);
$ns->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nsiorfile);
$sup->DeleteFile ($nsiorfile);
$con->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nfsiorfile);
$nfs->DeleteFile ($persistent_xml);
$nfs->DeleteFile ($persistent_000);

$NS = $ns->CreateProcess ("../../../Naming_Service/tao_cosnaming",
                                "-o $ns_nsiorfile");

$NFS = $nfs->CreateProcess ("../../../Notify_Service/tao_cosnotification");
$NFS_Args = "-ORBSvcConf $nfs_svc_conf -ORBInitRef NameService=file://$nfs_nsiorfile -IORoutput $nfs_nfsiorfile ";

$SUP = $sup->CreateProcess ("supplier");
$SUP_Args = "-ORBInitRef NameService=file://$sup_nsiorfile -ORBDebugLevel $debug_level";

$CON = $con->CreateProcess ("consumer");
$CON_Args = "-ORBInitRef NameService=file://$con_nsiorfile";

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

sub run_test
{
    my $client_args = shift;

    $nfs->DeleteFile ($nfsiorfile);

    $NFS->Arguments ($NFS_Args);
    $args = $NFS->Arguments ();
    print STDERR "Running Notification with arguments: $args\n";
    $NFS_status = $NFS->Spawn ();
    if ($NFS_status != 0) {
        print STDERR "ERROR: Notify Service returned $NFS_status\n";
        exit 1;
    }
    if ($nfs->WaitForFileTimed ($nfsiorfile,$nfs->ProcessStartWaitInterval()+$startup_timeout) == -1) {
        print STDERR "ERROR: cannot find file <$nfs_nfsiorfile>\n";
        $NS->Kill (); $NS->TimedWait (1);
        $NFS->Kill (); $NFS->TimedWait (1);
        exit 1;
    }
    sleep (5);

    $SUP->Arguments ($client_args . $SUP_Args);
    $CON->Arguments ($client_args . $CON_Args);
    $args = $SUP->Arguments ();
    print STDERR "Running Supplier with arguments: $args\n";
    $args = $CON->Arguments ();
    print STDERR "Running Consumer with arguments: $args\n";
    $SUP_status = $SUP->Spawn ();
    if ($SUP_status != 0) {
        print STDERR "ERROR: Supplier returned $SUP_status\n";
        $NS->Kill (); $NS->TimedWait (1);
        $NFS->Kill (); $NFS->TimedWait (1);
        exit 1;
    }
    $CON_status = $CON->Spawn ();
    if ($CON_status != 0) {
        print STDERR "ERROR: Consumer returned $CON_status\n";
        $SUP->Kill (); $SUP->TimedWait (1);
        $NS->Kill (); $NS->TimedWait (1);
        $NFS->Kill (); $NFS->TimedWait (1);
        exit 1;
    }

    $CON_status = $CON->WaitKill ($con->ProcessStartWaitInterval()+$experiment_timeout);
    if ($CON_status != 0) {
        print STDERR "ERROR: Consumer returned $CON_status\n";
        $CON->Kill (); $CON->TimedWait (1);
        $SUP->Kill (); $SUP->TimedWait (1);
        $NS->Kill (); $NS->TimedWait (1);
        $NFS->Kill (); $NFS->TimedWait (1);
        exit 1;
    }

    $SUP_status = $SUP->WaitKill ($sup->ProcessStartWaitInterval()+$experiment_timeout);
    if ($SUP_status != 0) {
        print STDERR "ERROR: Supplier returned $SUP_status\n";
        $SUP->Kill (); $SUP->TimedWait (1);
        $NS->Kill (); $NS->TimedWait (1);
        $NFS->Kill (); $NFS->TimedWait (1);
        exit 1;
    }


    $NFS_status = $NFS->Kill ($nfs->ProcessStopWaitInterval());
    if ($NFS_status != 0) {
        print STDERR "ERROR: Notify Service returned $NFS_status\n";
        $NS->Kill (); $NS->TimedWait (1);
        $NFS->Kill (); $NFS->TimedWait (1);
        exit 1;
    }
}

run_test ();
run_test ("-m -p ");


$NS_status = $NS->Kill ($ns->ProcessStopWaitInterval());
if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NFS_status\n";
    $status = 1;
}

$ns->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nsiorfile);
$sup->DeleteFile ($nsiorfile);
$con->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nfsiorfile);
$nfs->DeleteFile ($persistent_xml);
$nfs->DeleteFile ($persistent_000);

exit $status;
