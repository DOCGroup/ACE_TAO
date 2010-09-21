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
my $nfs = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $sup = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $con = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";

PerlACE::check_privilege_group();

# -n notify.conf -s supplier.conf -c consumer.conf -o output_dir -h
getopts ("n:s:c:o:h");

if ($opt_h) {
    print STDERR "-n notify.conf -s supplier.conf -c consumer.conf -o output_dir -h\n";
    exit 0;
}
#added to disable warning
$opt_h = 0;

$experiment_timeout = 240;
$startup_timeout = 120;

if ($opt_n) {
    $nfs_nfsconffile = $nfs->LocalFile ($opt_n);

}else {
    $nfs_nfsconffile = $nfs->LocalFile ("notify.conf");
}

if ($opt_s) {
    $sup_supconffile = $sup->LocalFile ($opt_s);

}else {
    $sup_supconffile = $sup->LocalFile ("supplier.conf");
}

if ($opt_c) {
    $con_conconffile = $con->LocalFile ($opt_c);

}else {
    $con_conconffile = $con->LocalFile ("consumer.conf");
}

$nsiorfile = "naming.ior";
$nfsiorfile = "notify.ior";
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
$con->DeleteFile ($supiorfile);

#TODO: remove all *.dat files when the test will be fixed
#@list=glob("*.dat");
#for $file (@list)
#{
#    DeleteFile($file) or die "Could not delete $file";
#}

$NS = $ns->CreateProcess ("../../../../../Naming_Service/Naming_Service",
                          "-o $ns_nsiorfile");
$NFS = $nfs->CreateProcess ("../../../../../Notify_Service/tao_cosnotification");
$NFS_Args = "-ORBInitRef NameService=file://$nfs_nsiorfile -IORoutput $nfs_nfsiorfile -ORBSvcConf $nfs_nfsconffile";

$SUP = $sup->CreateProcess ("../../../Driver/Notify_Tests_Driver");
$SUP_Args = "-ORBDebugLevel $debug_level -ORBInitRef NameService=file://$sup_nsiorfile -IORoutput $sup_supiorfile -ORBSvcConf $sup_supconffile";

$CON = $con->CreateProcess ("../../../Driver/Notify_Tests_Driver");
$CON_Args = "-ORBInitRef NameService=file://$con_nsiorfile -IORinput file://$con_supiorfile -ORBSvcConf $con_conconffile";

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

$NFS->Arguments ($NFS_Args);
$args = $NFS->Arguments ();
print STDERR "Running Notification with arguments: $args\n";
$NFS_status = $NFS->Spawn ();
if ($NFS_status != 0) {
    print STDERR "ERROR: Notification Service returned $NFS_status\n";
    $NFS->Kill (); $NFS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($nfs->WaitForFileTimed ($nfsiorfile,$nfs->ProcessStartWaitInterval()+$startup_timeout) == -1) {
    print STDERR "ERROR: cannot find file <$nfs_nfsiorfile>\n";
    $NFS->Kill (); $NFS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$SUP->Arguments ($SUP_Args);
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
    $NFS->Kill (); $NFS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
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

$CON->Arguments ($CON_Args);
$args = $CON->Arguments ();
print STDERR "Running Consumer with arguments: $args\n";
$CON_status = $CON->SpawnWaitKill ($con->ProcessStartWaitInterval()+$experiment_timeout);
if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $status = 1;
}


$SUP_status = $SUP->WaitKill ($sup->ProcessStopWaitInterval());
if ($SUP_status != 0) {
    print STDERR "ERROR: Closing Supplier returned $SUP_status\n";
    $status = 1;
}

$NFS_status = $NFS->TerminateWaitKill ($nfs->ProcessStopWaitInterval());
if ($NFS_status != 0) {
    print STDERR "ERROR: Closing Notification Service returned $NFS_status\n";
    $status = 1;
}

$NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval());
if ($NS_status != 0) {
    print STDERR "ERROR: Closing Name Service returned $NS_status\n";
    $status = 1;
}

if ($opt_o) {
    print STDERR "Results are not saved in $opt_o\n";
}

$ns->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nsiorfile);
$sup->DeleteFile ($nsiorfile);
$con->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nfsiorfile);
$sup->DeleteFile ($supiorfile);
$con->DeleteFile ($supiorfile);

exit $status;
