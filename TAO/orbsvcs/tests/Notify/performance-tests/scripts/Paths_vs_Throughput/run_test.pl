eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use File::Copy;
use Getopt::Std;

PerlACE::check_privilege_group();

$status = 0;

# -n notify.conf -s high_path.conf -c other_paths.conf -o output_dir -h
getopts ("n:s:c:o:h");

if ($opt_h) {
    $opt_h = 0; #to disable warning
    print STDERR "-n notify.conf -s high_path.conf -c other_paths.conf -o output_dir -h -d\n";
    exit 0;
}

$debug = "";

if ($opt_d) {
    $opt_d = 0; #to disable warning
    $debug = "-ORBDebugLevel 1"
}

$notify_conf = "notify.conf";
if ($opt_n) {
    $notify_conf = $opt_n;
}

$high_path_conf = "high_path.conf";
if ($opt_s) {
    $high_path_conf = $opt_s;
}

$other_paths_conf = "other_paths.conf";
if ($opt_c) {
    $other_paths_conf = $opt_c;
}

my $notify_ior    = "notify.ior";
my $naming_ior    = "naming.ior";
my $high_path_ior = "high_path.ior";

$nm_service = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
$nt_service = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
$high_path  = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
$other_path = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";

#uncomment it when test will be fixed
#@list = glob("*.dat");
#for $file (@list) {
#->DeleteFile($file);
#}

#localisations of ior files
$nm_service_nmiorfile = $nm_service->LocalFile($naming_ior);
$nt_service_nmiorfile = $nt_service->LocalFile($naming_ior);
$high_path_nmiorfile = $high_path->LocalFile($naming_ior);
$other_path_nmiorfile = $other_path->LocalFile($naming_ior);
$nt_service_ntiorfile = $nt_service->LocalFile($notify_ior);
$high_path_hpiorfile = $high_path->LocalFile($high_path_ior);
$other_path_hpiorfile = $other_path->LocalFile($high_path_ior);
#delete all ior files
$nm_service->DeleteFile($naming_ior);
$nt_service->DeleteFile($naming_ior);
$high_path->DeleteFile($naming_ior);
$other_path->DeleteFile($naming_ior);
$nt_service->DeleteFile($notify_ior);
$high_path->DeleteFile($high_path_ior);
$other_path->DeleteFile($high_path_ior);
#localisations of conf files
$nt_service_ntconffile = $nt_service->LocalFile($notify_conf);
$high_path_hpconffile = $high_path->LocalFile($high_path_conf);
$other_path_opconffile = $other_path->LocalFile($other_paths_conf);

$NM_SV = $nm_service->CreateProcess ("../../../../../Naming_Service/Naming_Service",
                                     "-o $nm_service_nmiorfile $debug");
$NT_SV = $nt_service->CreateProcess ("../../../../../Notify_Service/tao_cosnotification",
                                     "-ORBInitRef NameService=file://$nt_service_nmiorfile ".
                                     "-IORoutput $nt_service_ntiorfile ".
                                     "-ORBSvcConf $nt_service_ntconffile $debug");

$HP = $high_path->CreateProcess ("../../../Driver/Notify_Tests_Driver",
                                 "-ORBInitRef NameService=file://$high_path_nmiorfile ".
                                 "-IORoutput $high_path_hpiorfile ".
                                 "-ORBSvcConf $high_path_hpconffile $debug");

$OP = $other_path->CreateProcess ("../../../Driver/Notify_Tests_Driver",
                                  "-ORBInitRef NameService=file://$other_path_nmiorfile ".
                                  "-IORinput file://$other_path_hpiorfile ".
                                  "-ORBSvcConf $other_path_opconffile $debug");

$process_status = $NM_SV->Spawn ();

if ($process_status != 0) {
    print STDERR "ERROR: naming service returned $process_status\n";
    exit 1;
}

if ($nm_service->WaitForFileTimed ($naming_ior,
                                   $nm_service->ProcessStartWaitInterval() + 285) == -1) {
    print STDERR "ERROR: cannot find file <$nm_service_nmiorfile>\n";
    $NM_SV->Kill (); $NM_SV->TimedWait (1);
    exit 1;
}

if ($nm_service->GetFile ($naming_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$nm_service_nmiorfile>\n";
    $NM_SV->Kill (); $NM_SV->TimedWait (1);
    exit 1;
}

if ($nt_service->PutFile ($naming_ior) == -1) {
    print STDERR "ERROR: cannot set file <$nt_service_nmiorfile>\n";
    $NM_SV->Kill (); $NM_SV->TimedWait (1);
    exit 1;
}

if ($high_path->PutFile ($naming_ior) == -1) {
    print STDERR "ERROR: cannot set file <$high_path_nmiorfile>\n";
    $NM_SV->Kill (); $NM_SV->TimedWait (1);
    exit 1;
}

if ($other_path->PutFile ($naming_ior) == -1) {
    print STDERR "ERROR: cannot set file <$other_path_nmiorfile>\n";
    $NM_SV->Kill (); $NM_SV->TimedWait (1);
    exit 1;
}

$process_status = $NT_SV->Spawn ();

if ($process_status != 0) {
    print STDERR "ERROR: notify service returned $process_status\n";
    $NM_SV->Kill (); $NM_SV->TimedWait (1);
    exit 1;
}

if ($nt_service->WaitForFileTimed ($notify_ior,
                                   $nt_service->ProcessStartWaitInterval() + 285) == -1) {
    print STDERR "ERROR: cannot find file <$nt_service_ntiorfile> to start\n";
    $NM_SV->Kill (); $NM_SV->TimedWait (1);
    $NT_SV->Kill (); $NT_SV->TimedWait (1);
    exit 1;
}

$process_status = $HP->Spawn ();

if ($process_status != 0) {
    print STDERR "ERROR: high path returned $process_status\n";
    $NM_SV->Kill (); $NM_SV->TimedWait (1);
    $NT_SV->Kill (); $NT_SV->TimedWait (1);
    exit 1;
}

if ($high_path->WaitForFileTimed ($high_path_ior,
                                  $high_path->ProcessStartWaitInterval() + 285) == -1) {
    print STDERR "ERROR: cannot find file <$high_path_hpiorfile> to start\n";
    $NM_SV->Kill (); $NM_SV->TimedWait (1);
    $NT_SV->Kill (); $NT_SV->TimedWait (1);
    $HP->Kill (); $HP->TimedWait (1);
    exit 1;
}

$process_status = $OP->SpawnWaitKill ($other_path->ProcessStartWaitInterval() + 285);

if ($process_status != 0) {
    print STDERR "ERROR: other path returned $process_status\n";
    $status = 1;
    $HP->Kill (); $HP->TimedWait (1);
    $NT_SV->Kill (); $NT_SV->TimedWait (1);
    $NM_SV->Kill (); $NM_SV->TimedWait (1);
    exit 1;
}

$process_status = $HP->WaitKill ($high_path->ProcessStopWaitInterval());
if ($process_status != 0) {
    print STDERR "ERROR: high path returned $process_status\n";
    $status = 1;
}

$NT_SV->Kill ();
$NM_SV->Kill ();

if ($opt_o) {
    print STDERR "Results are not saved in $opt_o\n";
}

$nm_service->DeleteFile($naming_ior);
$nt_service->DeleteFile($naming_ior);
$high_path->DeleteFile($naming_ior);
$other_path->DeleteFile($naming_ior);
$nt_service->DeleteFile($notify_ior);
$high_path->DeleteFile($high_path_ior);
$other_path->DeleteFile($high_path_ior);

exit $status;
