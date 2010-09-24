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

my $exec_extn = "";
if ($^O eq "MSWin32") {
    $exec_extn = ".exe";
}

my @nslist_paths = (["../../../../bin", ""],
                    ["../../../bin", ""],
                    ["../../../../TAO/utils/nslist", ""]);
if (grep(($_ eq 'ARCH'), @PerlACE::ConfigList::Configs)) {
    push @nslist_paths, ["../../../../bin/" . $PerlACE::Process::ExeSubDir,
                         "../../../../bin"];
}

my $nslist_path = "";
for my $p (@nslist_paths) {
  my $use_path = ($p->[1] eq "") ? $p->[0] : $p->[1];
  if (-e $p->[0] . '/tao_nslist') {
    $nslist_path = $use_path . '/tao_nslist';
    last;
  }
  if ($exec_extn ne "" && -e $p->[0] . '/tao_nslist' . $exec_extn) {
    $nslist_path = $use_path . '/tao_nslist' . $exec_extn;
    last;
  }
}

if ($nslist_path eq "") {
    print STDERR "ERROR: tao_nslist utility not found.\n";
    exit 1;
}

my $ns_service = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $ns_list = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "ns.ior";
my $ns_service_iorfile = $ns_service->LocalFile ($iorbase);
my $ns_list_iorfile = $ns_list->LocalFile ($iorbase);
$ns_service->DeleteFile($iorbase);
$ns_list->DeleteFile($iorbase);

$NS = $ns_service->CreateProcess ("../../../../TAO/orbsvcs/Naming_Service/tao_cosnaming",
                                  "-ORBdebuglevel $debug_level -o $ns_service_iorfile ".
                                  "-ORBObjRefStyle URL -ORBEndpoint shmiop:// -ORBSvcConfDirective ".
                                  "\"dynamic SHMIOP_Factory Service_Object ".
                                  "*TAO_Strategies:_make_TAO_SHMIOP_Protocol_Factory () ''\"");

$NSL = $ns_list->CreateProcess ($nslist_path, "-ORBSvcConfDirective \"dynamic SHMIOP_Factory Service_Object ".
                                              "*TAO_Strategies:_make_TAO_SHMIOP_Protocol_Factory () ''\" ".
                                              "-ORBInitRef NameService=file://$ns_list_iorfile --ior --ctxior ".
                                              "--node \"\" --tree \"\"");

$process_status = $NS->Spawn ();

if ($process_status != 0) {
    print STDERR "ERROR: naming service returned $process_status\n";
    exit 1;
}

if ($ns_service->WaitForFileTimed ($iorbase,
                                   $ns_service->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ns_service_iorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

if ($ns_service->GetFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ns_service_iorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

if ($ns_list->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$ns_list_iorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$process_status = $NSL->SpawnWaitKill ($ns_list->ProcessStartWaitInterval() + 285);

if ($process_status != 0) {
    print STDERR "ERROR: Regression - tao_nslist returned $process_status\n";
    $status = 1;
}

$process_status = $NS->TerminateWaitKill ($ns_service->ProcessStopWaitInterval());

if ($process_status != 0) {
    print STDERR "ERROR: naming service TerminateWaitKill returned $process_status\n";
    $status = 1;
}

$ns_service->DeleteFile($iorbase);
$ns_list->DeleteFile($iorbase);

if ($status == 0) {
    print "Test passed !!\n";
}

exit $status;
