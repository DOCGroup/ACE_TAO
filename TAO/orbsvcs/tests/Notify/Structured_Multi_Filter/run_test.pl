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
my $con_supiorfile = $con->LocalFile ($supiorfile);
my $nfs_nfsconffile = $nfs->LocalFile ($nfsconffile);
$ns->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nfsiorfile);
$sup->DeleteFile ($supiorfile);
$con->DeleteFile ($supiorfile);

$NS = $ns->CreateProcess ("../../../Naming_Service/tao_cosnaming",
                            "-ORBEndpoint iiop://$host:$port ".
                            "-o $ns_nsiorfile");
$NFS = $nfs->CreateProcess ("../../../Notify_Service/tao_cosnotification",
                            "-ORBInitRef NameService=iioploc://" .
                            "$host:$port/NameService " .
                            "-IORoutput $nfs_nfsiorfile -ORBSvcConf " .
                            "$nfs_nfsconffile");
$SUP = $sup->CreateProcess ("Structured_Supplier",
                            "-ORBDebugLevel $debug_level ".
                             "-ORBInitRef NameService=iioploc://" .
                             "$host:$port/NameService ".
                             "-o $sup_supiorfile");
$CON = $con->CreateProcess ("Structured_Consumer",
                            "-ORBInitRef NameService=iioploc://".
                            "$host:$port/NameService ".
                            "-k file://$con_supiorfile");

my @ops = (undef, "AND", "OR");

$NS_status = $NS->Spawn ();
if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($ns->WaitForFileTimed ($nsiorfile,$nfs->ProcessStartWaitInterval()) == -1) {
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
if ($nfs->WaitForFileTimed ($nfsiorfile,$nfs->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$nfs_nfsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

foreach my $supplier_op (@ops) {
    foreach my $consumer_op (@ops) {
        my $supplier_args = "";
        if (defined $supplier_op) {
          $supplier_args .= " -f $supplier_op";
        }

        my $consumer_args = "";
        if (defined $supplier_op) {
          $consumer_args .= " -s $supplier_op";
        }
        if (defined $consumer_op) {
          $consumer_args .= " -f $consumer_op";
        }

        print "\n**** Testing with $consumer_args ****\n";

        $SUP->Arguments($SUP->Arguments() . $supplier_args);
        $SUP_status = $SUP->Spawn ();
        if ($SUP_status != 0) {
            print STDERR "ERROR: Supplier returned $SUP_status\n";
            $SUP->Kill (); $SUP->TimedWait (1);
            $NFS->Kill (); $NFS->TimedWait (1);
            $NS->Kill (); $NS->TimedWait (1);
            last;
        }
        if ($sup->WaitForFileTimed ($supiorfile,$sup->ProcessStartWaitInterval()) == -1) {
            print STDERR "ERROR: cannot find file <$sup_supiorfile>\n";
            $SUP->Kill (); $SUP->TimedWait (1);
            $NS->Kill (); $NS->TimedWait (1);
            $NFS->Kill (); $NFS->TimedWait (1);
            last;
        }
        if ($sup->GetFile ($supiorfile) == -1) {
            print STDERR "ERROR: cannot retrieve file <$sup_supiorfile>\n";
            $SUP->Kill (); $SUP->TimedWait (1);
            $NFS->Kill (); $NFS->TimedWait (1);
            $NS->Kill (); $NS->TimedWait (1);
            last;
        }
        if ($con->PutFile ($supiorfile) == -1) {
            print STDERR "ERROR: cannot set file <$con_supiorfile>\n";
            $SUP->Kill (); $SUP->TimedWait (1);
            $NFS->Kill (); $NFS->TimedWait (1);
            $NS->Kill (); $NS->TimedWait (1);
            last;
        }

        $CON->Arguments($CON->Arguments() . $consumer_args);
        $CON_status = $CON->SpawnWaitKill ($con->ProcessStartWaitInterval()+5);
        if ($CON_status != 0) {
            print STDERR "ERROR: Consumer returned $CON_status\n";
            $SUP->Kill (); $SUP->TimedWait (1);
            $NFS->Kill (); $NFS->TimedWait (1);
            $NS->Kill (); $NS->TimedWait (1);
            last;
        }

        $SUP_status = $SUP->WaitKill ($con->ProcessStopWaitInterval());
        if ($SUP_status != 0) {
            print STDERR "ERROR: Supplier returned $SUP_status\n";
            $NFS->Kill (); $NFS->TimedWait (1);
            $NS->Kill (); $NS->TimedWait (1);
            last;
        }

        $sup->DeleteFile ($supiorfile);
        $con->DeleteFile ($supiorfile);
    }
}

$NFS_status = $NFS->Kill ($nfs->ProcessStopWaitInterval());
if ($NFS_status != 0) {
    print STDERR "ERROR: Notify Service returned $NFS_status\n";
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
$con->DeleteFile ($supiorfile);

exit $status;
