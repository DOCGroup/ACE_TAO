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

$port = $ns->RandomPort ();
$host = $ns->HostName ();

$supiorfile = "supplier.ior";
$nfsiorfile = "notify.ior";
$nfsconffile = "notify$PerlACE::svcconf_ext";

my $nfs_nfsiorfile = $nfs->LocalFile ($nfsiorfile);
my $sup_supiorfile = $sup->LocalFile ($supiorfile);
my $con_supiorfile = $con->LocalFile ($supiorfile);
my $nfs_nfsconffile = $nfs->LocalFile ($nfsconffile);
$nfs->DeleteFile ($nfsiorfile);
$sup->DeleteFile ($supiorfile);
$con->DeleteFile ($supiorfile);

$NS = $ns->CreateProcess ("../../../Naming_Service/Naming_Service",
                            "-ORBEndpoint iiop://$host:$port");
$NFS = $nfs->CreateProcess ("../../../Notify_Service/Notify_Service",
                            "-ORBInitRef NameService=iioploc://" .
                            "$host:$port/NameService " .
                            "-IORoutput $nfs_nfsiorfile -ORBSvcConf " .
                            "$nfs_nfsconffile");
$SUP = $sup->CreateProcess ("Sequence_Supplier",
                            "-ORBDebugLevel $debug_level ".
                             "-ORBInitRef NameService=iioploc://" .
                             "$host:$port/NameService ".
                             "-o $sup_supiorfile");
$CON = $con->CreateProcess ("Sequence_Consumer");

$client_args = "-ORBInitRef NameService=iioploc://".
               "$host:$port/NameService ".
               "-k file://$con_supiorfile";

$NS_status = $NS->Spawn ();
if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
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

@low = (40, 20, 20, 20);
@high = (40, 21, 21, 21);
@constraintList = (
    "\"Number == 20 or Number == 10\"",
    "\"Number == 20 or Index == 7\"",
    "\"Number == 20 or Index == 4\"",
    "\"Number == 10 or (Number == 20 and Index == 1)\"",
);

$i = 0;

foreach $constraintString (@constraintList) {
    print "<*><*><*> Constraint String: $constraintString  <*><*><*>\n";

    $sup->DeleteFile ($supiorfile);
    $con->DeleteFile ($supiorfile);
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


    $CON->Arguments($client_args . " -l $low[$i] -h $high[$i] -c $constraintString");

    $i = $i + 1;

    $CON_status = $CON->SpawnWaitKill ($con->ProcessStartWaitInterval()+45);
    if ($CON_status != 0) {
        print STDERR "ERROR: Consumer returned $CON_status\n";
        last;
   }

    $SUP_status = $SUP->WaitKill ($con->ProcessStopWaitInterval());
    if ($SUP_status != 0) {
        print STDERR "ERROR: Supplier returned $SUP_status\n";
        exit 1;
    }
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

$nfs->DeleteFile ($nfsiorfile);
$sup->DeleteFile ($supiorfile);
$con->DeleteFile ($supiorfile);

exit $status;
