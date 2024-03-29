eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

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

$experiment_timeout = 60;
$startup_timeout = 60;
$shutdown_timeout = 10;

$nfsiorfile = "notify.ior";
$nsiorfile = "naming.ior";
$supiorfile = "supplier.ior";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $nfs_nsiorfile = $nfs->LocalFile ($nsiorfile);
my $sup_nsiorfile = $sup->LocalFile ($nsiorfile);
my $con_nsiorfile = $con->LocalFile ($nsiorfile);
my $nfs_nfsiorfile = $nfs->LocalFile ($nfsiorfile);
my $sup_supiorfile = $sup->LocalFile ($supiorfile);
my $con_supiorfile = $con->LocalFile ($supiorfile);
$ns->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nsiorfile);
$sup->DeleteFile ($nsiorfile);
$con->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nfsiorfile);
$sup->DeleteFile ($supiorfile);
$con->DeleteFile ($coniorfile);

@tests = ( {
    description => "no threads",
    config => "notify_nothreads$PerlACE::svcconf_ext",
    supplier => " -c 10 -e 10 ",
    consumer => " -c 2 -e 100 ",
   }, {
    description => "multi-threaded dispatching",
    config => "notify_mtdispatching$PerlACE::svcconf_ext",
    supplier => " -c 10 -e 10 ",
    consumer => " -c 2 -e 100 ",
   }, {
    description => "multi-threaded supplier-side",
    config => "notify_mtsource$PerlACE::svcconf_ext",
    supplier => " -c 10 -e 10 ",
    consumer => " -c 2 -e 100 ",
   },
);

$NS = $ns->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Naming_Service/tao_cosnaming",
                                "-o $ns_nsiorfile");

$NFS = $nfs->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Notify_Service/tao_cosnotification");
$NFS_Args = "-ORBInitRef NameService=file://$nfs_nsiorfile ".
            " -IORoutput $nfs_nfsiorfile";

$SUP = $sup->CreateProcess ("Structured_Supplier",
                            "-ORBDebugLevel $debug_level ".
                            "-ORBInitRef NameService=file://$sup_nsiorfile ".
                            "-o $sup_supiorfile");

$CON = $con->CreateProcess ("Structured_Consumer",
                            "-ORBInitRef NameService=file://$con_nsiorfile ".
                            "-k file://$con_supiorfile");

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

for $test (@tests) {
    print STDERR "\nTesting $test->{description} ....\n\n";

    $nfs->DeleteFile ($nfsiorfile);
    my $config = $test->{config};
    my $nfs_config = $nfs->LocalFile ($config);

    $NFS->Arguments ($NFS_Args . " -ORBSvcConf $nfs_config");
    $args = $NFS->Arguments ();
    print STDERR "Running Notification with arguments: $args\n";
    $NFS_status = $NFS->Spawn ();
    if ($NFS_status != 0) {
        print STDERR "ERROR: Notify Service returned $NFS_status\n";
        $NS->Kill (); $NS->TimedWait (1);
        $NFS->Kill (); $NFS->TimedWait (1);
        exit 1;
    }
    if ($nfs->WaitForFileTimed ($nfsiorfile,$nfs->ProcessStartWaitInterval()+$startup_timeout) == -1) {
        print STDERR "ERROR: cannot find file <$nfs_nfsiorfile>\n";
        $NS->Kill (); $NS->TimedWait (1);
        $NFS->Kill (); $NFS->TimedWait (1);
        exit 1;
    }

    $sup->DeleteFile ($supiorfile);
    $con->DeleteFile ($coniorfile);
    $SUP->Arguments ($SUP->Arguments () . $test->{supplier});
    $args = $SUP->Arguments ();
    print STDERR "Running Supplier with arguments: $args\n";
    $SUP_status = $SUP->Spawn ();
    if ($SUP_status != 0) {
        print STDERR "ERROR: Supplier returned $SUP_status\n";
        $SUP->Kill (); $SUP->TimedWait (1);
        $NFS->Kill (); $NFS->TimedWait (1);
        $NS->Kill (); $NS->TimedWait (1);
        exit 1;
    }
    if ($sup->WaitForFileTimed ($supiorfile,$sup->ProcessStartWaitInterval()+$startup_timeout) == -1) {
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
    if ($con->PutFile ($supiorfile) == -1) {
        print STDERR "ERROR: cannot set file <$con_supiorfile>\n";
        $SUP->Kill (); $SUP->TimedWait (1);
        $NFS->Kill (); $NFS->TimedWait (1);
        $NS->Kill (); $NS->TimedWait (1);
        exit 1;
    }

    $CON->Arguments ($CON->Arguments () . $test->{consumer});
    $args = $CON->Arguments ();
    print STDERR "Running Consumer with arguments: $args\n";
    $CON_status = $CON->SpawnWaitKill ($con->ProcessStartWaitInterval()+$experiment_timeout);
    if ($CON_status != 0) {
        print STDERR "ERROR: Consumer returned $CON_status\n";
        $SUP->Kill (); $SUP->TimedWait (1);
        $NFS->Kill (); $NFS->TimedWait (1);
        $NS->Kill (); $NS->TimedWait (1);
        exit 1;
    }

    $SUP_status = $SUP->WaitKill ($shutdown_timeout);
    if ($SUP_status != 0) {
        print STDERR "ERROR: Supplier returned $SUP_status\n";
        $NFS->Kill (); $NFS->TimedWait (1);
        $NS->Kill (); $NS->TimedWait (1);
        exit 1;
    }

    $NFS_status = $NFS->Kill ($nfs->ProcessStopWaitInterval());
    if ($NFS_status != 0) {
        print STDERR "ERROR: Notify Service returned $NFS_status\n";
        $NS->Kill (); $NS->TimedWait (1);
        exit 1;
    }
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
$nfs->DeleteFile ($nfsiorfile);
$sup->DeleteFile ($supiorfile);
$con->DeleteFile ($coniorfile);

exit $status;
