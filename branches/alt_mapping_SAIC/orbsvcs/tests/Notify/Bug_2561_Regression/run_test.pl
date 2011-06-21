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
my $con1 = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $con2 = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";

$sup->AddLibPath ('../lib');
$con1->AddLibPath ('../lib');
$con2->AddLibPath ('../lib');

PerlACE::check_privilege_group();

$port = $ns->RandomPort ();
$host = $ns->HostName ();

$supiorfile = "supplier.ior";
$nsiorfile = "naming.ior";
$nfsiorfile = "notify.ior";
$nfsconffile = "notify$PerlACE::svcconf_ext";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $nfs_nfsiorfile = $nfs->LocalFile ($nfsiorfile);
my $sup_supiorfile = $sup->LocalFile ($supiorfile);
my $con1_supiorfile = $con1->LocalFile ($supiorfile);
my $con2_supiorfile = $con2->LocalFile ($supiorfile);
my $nfs_nfsconffile = $nfs->LocalFile ($nfsconffile);
$ns->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nfsiorfile);
$sup->DeleteFile ($supiorfile);
$con1->DeleteFile ($supiorfile);
$con2->DeleteFile ($supiorfile);

$NS = $ns->CreateProcess ("../../../Naming_Service/tao_cosnaming",
                            "-ORBEndpoint iiop://$host:$port ".
                            "-o $ns_nsiorfile");
$NFS = $nfs->CreateProcess ("../../../Notify_Service/tao_cosnotification",
                            "-ORBInitRef NameService=iioploc://" .
                            "$host:$port/NameService " .
                            "-IORoutput $nfs_nfsiorfile -ORBSvcConf " .
                            "$nfs_nfsconffile");
$SUP = $sup->CreateProcess ("Supplier",
                            "-ORBDebugLevel $debug_level ".
                             "-ORBInitRef NameService=iioploc://" .
                             "$host:$port/NameService ".
                             "-o $sup_supiorfile ");
$CON1 = $con1->CreateProcess ("Consumer",
                            "-ORBInitRef NameService=iioploc://".
                            "$host:$port/NameService ".
                            "-k file://$con1_supiorfile");
$CON2 = $con2->CreateProcess ("Consumer",
                            "-ORBInitRef NameService=iioploc://".
                            "$host:$port/NameService -g ".
                            "-k file://$con2_supiorfile");

$NS_status = $NS->Spawn ();
if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($ns->WaitForFileTimed ($nsiorfile,$ns->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ns_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$NFS_status = $NFS->Spawn ();
if ($NFS_status != 0) {
    print STDERR "ERROR: Notify Service returned $NFS_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($nfs->WaitForFileTimed ($nfsiorfile,$nfs->ProcessStartWaitInterval()+5) == -1) {
    print STDERR "ERROR: cannot find file <$nfs_nfsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

$SUP_status = $SUP->Spawn ();
if ($SUP_status != 0) {
    print STDERR "ERROR: Supplier returned $SUP_status\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($sup->WaitForFileTimed ($supiorfile,$sup->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$sup_supiorfile>\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($sup->GetFile ($supiorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$sup_supiorfile>\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($con1->PutFile ($supiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$con1_supiorfile>\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($con2->PutFile ($supiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$con2_supiorfile>\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$CON1_status = $CON1->Spawn ();
if ($CON1_status != 0) {
    print STDERR "ERROR: Consumer 1 returned $CON1_status\n";
    $CON1->Kill (); $CON1->TimedWait (1);
    $SUP->Kill (); $SUP->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

# Give the STC1 time to start up before STC2 starts and triggers the supplier to send.
sleep(2);

$CON2_status = $CON2->SpawnWaitKill ($con2->ProcessStartWaitInterval()+5);
if ($CON2_status != 0) {
    print STDERR "ERROR: Consumer 2 returned $CON2_status\n";
    $status = 1;
}

$CON1_status = $CON1->WaitKill ($con1->ProcessStartWaitInterval()+5);
if ($CON1_status != 0) {
    print STDERR "ERROR: Consumer 1 returned $CON1_status\n";
    $status = 1;
}


$SUP_status = $SUP->WaitKill ($sup->ProcessStopWaitInterval());
if ($SUP_status != 0) {
    print STDERR "ERROR: Supplier returned $SUP_status\n";
    $status = 1;
}

$NFS_status = $NFS->Kill ($nfs->ProcessStopWaitInterval());
if ($NFS_status != 0) {
    print STDERR "ERROR: Notify Service returned $NFS_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    $status = 1;
}

$NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval());
if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    $status = 1;
}

$ns->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nfsiorfile);
$sup->DeleteFile ($supiorfile);
$con1->DeleteFile ($supiorfile);
$con2->DeleteFile ($supiorfile);

exit $status;
