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

my $nslist_path = "$ENV{ACE_ROOT}/bin/tao_nslist";

## The two test targets will communicate over shared memory (SHMIOP) so they
## can't be 32-bit and 64-bit executables on the same host.
my $target_ns_list = 2;
if (exists $ENV{'DOC_TEST_2_EXE_SUBDIR'} && $ENV{'DOC_TEST_2_OS'} eq 'local' &&
    $ENV{'DOC_TEST_2_EXE_SUBDIR'} ne $PerlACE::Process::ExeSubDir) {
  $target_ns_list = 1;
}

my $ns_service = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $ns_list = PerlACE::TestTarget::create_target ($target_ns_list) || die "Create target $target_ns_list failed\n";

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
