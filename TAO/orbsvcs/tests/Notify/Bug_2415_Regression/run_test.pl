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

$sup->AddLibPath ('../lib');
$con->AddLibPath ('../lib');

PerlACE::check_privilege_group();

$domain1 = "domain1";
$domain2 = "domain2";
$expect_pass = "0";
$expect_fail = "1";
$ns_port = $ns->RandomPort ();
$ns_host = $ns->HostName ();

$supiorfile = "supplier.ior";
$nfsiorfile = "notify.ior";
$nfsconffile = "notify$PerlACE::svcconf_ext";
$nfs2conffile = "notify2$PerlACE::svcconf_ext";

my $nfs_nfsiorfile = $nfs->LocalFile ($nfsiorfile);
my $nfs_nfsconffile = $nfs->LocalFile ($nfsconffile);
my $nfs_nfs2conffile = $nfs->LocalFile ($nfs2conffile);
my $sup_supiorfile = $sup->LocalFile ($supiorfile);
my $con_supiorfile = $con->LocalFile ($supiorfile);
$nfs->DeleteFile ($nfsiorfile);
$sup->DeleteFile ($supiorfile);
$con->DeleteFile ($supiorfile);

$NS = $ns->CreateProcess ("../../../Naming_Service/tao_cosnaming",
                          " -ORBEndpoint iiop://$ns_host:$ns_port");

$NFS = $nfs->CreateProcess ("../../../Notify_Service/tao_cosnotification",
                            "-ORBInitRef NameService=corbaloc::" .
                            "$ns_host:$ns_port/NameService " .
                            "-IORoutput $nfs_nfsiorfile -ORBSvcConf " .
                            "$nfs_nfsconffile");

$SUP = $sup->CreateProcess ("Supplier",
                            "-ORBDebugLevel $debug_level " .
                            "-ORBInitRef NameService=iioploc://" .
                            "$ns_host:$ns_port/NameService -d $domain1 ".
                            "-o $sup_supiorfile");

$CON = $con->CreateProcess ("Consumer");
$CON_Args = "-ORBInitRef NameService=corbaloc::$ns_host:" .
            "$ns_port/NameService -f $expect_pass ".
            "-k file://$con_supiorfile";

$NS_status = $NS->Spawn ();
if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    exit 1;
}

$NFS_status = $NFS->Spawn ();
if ($NFS_status != 0) {
    print STDERR "ERROR: Notify Service returned $NFS_status\n";
    exit 1;
}
if ($nfs->WaitForFileTimed ($nfsiorfile,$nfs->ProcessStartWaitInterval()+5) == -1) {
    print STDERR "ERROR: cannot find file <$nfs_nfsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

print "************** Running Consumer 1 ************\n";

$SUP_status = $SUP->Spawn ();
if ($SUP_status != 0) {
    print STDERR "ERROR: Supplier returned $SUP_status\n";
    exit 1;
}
if ($sup->WaitForFileTimed ($supiorfile,$sup->ProcessStartWaitInterval()+5) == -1) {
    print STDERR "ERROR: cannot find file <$sup_supiorfile>\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($sup->GetFile ($supiorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$sup_supiorfile>\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($con->PutFile ($supiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$con_suporfile>\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

$CON->Arguments($CON_Args);
$CON_status = $CON->SpawnWaitKill ($con->ProcessStartWaitInterval()+45);
if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

$SUP_status = $SUP->WaitKill ($sup->ProcessStopWaitInterval());
if ($SUP_status != 0) {
    print STDERR "ERROR: Supplier returned $SUP_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

$sup->DeleteFile ($supiorfile);
$con->DeleteFile ($supiorfile);

$SUP = $sup->CreateProcess ("Supplier",
                            "-ORBDebugLevel $debug_level " .
                            "-ORBInitRef NameService=iioploc://" .
                            "$ns_host:$ns_port/NameService -d $domain2 ".
                            "-o $sup_supiorfile");

$CON = $con->CreateProcess ("Consumer");
$CON_Args = "-ORBInitRef NameService=corbaloc::$ns_host:" .
            "$ns_port/NameService -f $expect_fail ".
            "-k file://$con_supiorfile";

print "************** Running Consumer 2 ************\n";

$SUP_status = $SUP->Spawn ();
if ($SUP_status != 0) {
    print STDERR "ERROR: Supplier returned $SUP_status\n";
    exit 1;
}
if ($sup->WaitForFileTimed ($supiorfile,$sup->ProcessStartWaitInterval()+5) == -1) {
    print STDERR "ERROR: cannot find file <$sup_supiorfile>\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($sup->GetFile ($supiorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$sup_supiorfile>\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($con->PutFile ($supiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$con_suporfile>\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

$CON->Arguments($CON_Args);
$CON_status = $CON->SpawnWaitKill ($con->ProcessStartWaitInterval()+45);
if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

$SUP_status = $SUP->WaitKill ($sup->ProcessStopWaitInterval());
if ($SUP_status != 0) {
    print STDERR "ERROR: Supplier returned $SUP_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

$NFS_status = $NFS->Kill ($nfs->ProcessStopWaitInterval());
if ($NFS_status != 0) {
    print STDERR "ERROR: Notify Service returned $NFS_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$sup->DeleteFile ($supiorfile);
$con->DeleteFile ($supiorfile);
$nfs->DeleteFile ($nfsiorfile);

$NFS = $nfs->CreateProcess ("../../../Notify_Service/tao_cosnotification",
                            "-ORBInitRef NameService=corbaloc::" .
                            "$ns_host:$ns_port/NameService " .
                            "-IORoutput $nfs_nfsiorfile -ORBSvcConf " .
                            "$nfs_nfs2conffile");

$SUP = $sup->CreateProcess ("Supplier",
                            "-ORBDebugLevel $debug_level " .
                            "-ORBInitRef NameService=iioploc://" .
                            "$ns_host:$ns_port/NameService -d $domain1 ".
                            "-o $sup_supiorfile");

$CON = $con->CreateProcess ("Consumer");
$CON_Args = "-ORBInitRef NameService=corbaloc::$ns_host:" .
            "$ns_port/NameService -f $expect_pass ".
            "-k file://$con_supiorfile";


$NFS_status = $NFS->Spawn ();
if ($NFS_status != 0) {
    print STDERR "ERROR: Notify Service returned $NFS_status\n";
    exit 1;
}
if ($nfs->WaitForFileTimed ($nfsiorfile,$nfs->ProcessStartWaitInterval()+5) == -1) {
    print STDERR "ERROR: cannot find file <$nfs_nfsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

print "************** Running Consumer 3 ************\n";

$SUP_status = $SUP->Spawn ();
if ($SUP_status != 0) {
    print STDERR "ERROR: Supplier returned $SUP_status\n";
    exit 1;
}
if ($sup->WaitForFileTimed ($supiorfile,$sup->ProcessStartWaitInterval()+5) == -1) {
    print STDERR "ERROR: cannot find file <$sup_supiorfile>\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($sup->GetFile ($supiorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$sup_supiorfile>\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($con->PutFile ($supiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$con_suporfile>\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

$CON->Arguments($CON_Args);
$CON_status = $CON->SpawnWaitKill ($con->ProcessStartWaitInterval()+45);
if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

$SUP_status = $SUP->WaitKill ($sup->ProcessStopWaitInterval());
if ($SUP_status != 0) {
    print STDERR "ERROR: Supplier returned $SUP_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

$sup->DeleteFile ($supiorfile);
$con->DeleteFile ($supiorfile);

$SUP = $sup->CreateProcess ("Supplier",
                            "-ORBDebugLevel $debug_level " .
                            "-ORBInitRef NameService=iioploc://" .
                            "$ns_host:$ns_port/NameService -d $domain2 ".
                            "-o $sup_supiorfile");

$CON = $con->CreateProcess ("Consumer");
$CON_Args = "-ORBInitRef NameService=corbaloc::$ns_host:" .
            "$ns_port/NameService -f $expect_pass ".
            "-k file://$con_supiorfile";

print "************** Running Consumer 4 ************\n";

$SUP_status = $SUP->Spawn ();
if ($SUP_status != 0) {
    print STDERR "ERROR: Supplier returned $SUP_status\n";
    exit 1;
}
if ($sup->WaitForFileTimed ($supiorfile,$sup->ProcessStartWaitInterval()+5) == -1) {
    print STDERR "ERROR: cannot find file <$sup_supiorfile>\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($sup->GetFile ($supiorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$sup_supiorfile>\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($con->PutFile ($supiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$con_suporfile>\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

$CON->Arguments($CON_Args);
$CON_status = $CON->SpawnWaitKill ($con->ProcessStartWaitInterval()+45);
if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
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
    $status = 1;
}

$NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval());
if ($NS_status != 0) {
    print STDERR "ERROR: Naming Service returned $NS_status\n";
    $status = 1;
}

$nfs->DeleteFile ($nfsiorfile);
$sup->DeleteFile ($supiorfile);
$con->DeleteFile ($supiorfile);

exit $status;
