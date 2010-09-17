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
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

# -n notify.conf -s high_path.conf -c other_paths.conf -o output_dir -h
getopts ("n:s:c:o:h");

if ($opt_h) {
    $opt_h = 0; #to disable fuxx.pl warning only
    print STDERR "-n notify.conf -s high_path.conf -c other_paths.conf -o output_dir -h\n";
    exit 0;
}

$nt_conf = "notify.conf";
if ($opt_n) {
    $nt_conf = $opt_n;
}

$high_path_conf = "high_path.conf";
if ($opt_s) {
    $high_path_conf = $opt_s;
}

$other_paths_conf = "other_paths.conf";
if ($opt_c) {
    $other_paths_conf = $opt_c;
}

my $nm_service = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $nt_service = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

#uncomment it when test will be fixed
#@list = glob("*.dat");
#for $file (@list) {
#    $nm_service->DeleteFile ($file);
#    $nt_service->DeleteFile ($file);
#}

my $nm_ior = "naming.ior";
my $nt_ior = "notify.ior";

my $nt_service_nt_conf = $nt_service->LocalFile($nt_conf);
my $nm_service_nm_ior  = $nm_service->LocalFile($nm_ior);
my $nt_service_nm_ior  = $nt_service->LocalFile($nm_ior);
my $nt_service_nt_ior  = $nt_service->LocalFile($nt_ior);
$nm_service->DeleteFile($nm_ior);
$nt_service->DeleteFile($nm_ior);
$nt_service->DeleteFile($nt_ior);

$NM_SV = $nm_service->CreateProcess ("../../../../../Naming_Service/Naming_Service",
                                     "-ORBDebugLevel $debug_level -o $nm_service_nm_ior");

$NT_SV = $nt_service->CreateProcess ("../../../../../Notify_Service/Notify_Service",
                                     "-ORBInitRef NameService=file://$nt_service_nm_ior ".
                                     "-IORoutput $nt_service_nt_ior ".
                                     "-ORBSvcConf $nt_service_nt_conf ".
                                     "-ORBDebugLevel $debug_level");

$service_status = $NM_SV->Spawn ();

if ($service_status != 0) {
    print STDERR "ERROR: naming service returned $service_status\n";
    exit 1;
}

if ($nm_service->WaitForFileTimed ($nm_ior,
                                   $nm_service->ProcessStartWaitInterval() + 105) == -1) {
    print STDERR "ERROR: cannot find file <$nm_service_nm_ior>\n";
    $NM_SV->Kill (); $NM_SV->TimedWait (1);
    exit 1;
}

if ($nm_service->GetFile ($nm_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$nm_service_nm_ior>\n";
    $NM_SV->Kill (); $NM_SV->TimedWait (1);
    exit 1;
}

if ($nt_service->PutFile ($nm_ior) == -1) {
    print STDERR "ERROR: cannot set file <$nt_service_nm_ior>\n";
    $NM_SV->Kill (); $NM_SV->TimedWait (1);
    exit 1;
}

$service_status = $NT_SV->SpawnWaitKill ($nt_service->ProcessStartWaitInterval() + 585);

if ($service_status != 0) {
    print STDERR "ERROR: notification service returned $service_status\n";
    exit 1;
}

$nm_service->DeleteFile($nm_ior);
$nt_service->DeleteFile($nm_ior);
$nt_service->DeleteFile($nt_ior);

exit $status;
