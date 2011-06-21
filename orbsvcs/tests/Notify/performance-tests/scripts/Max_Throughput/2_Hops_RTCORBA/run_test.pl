eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use File::Copy;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $nm_service = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $supplier = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $consumer = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
$supplier->AddLibPath("$ENV{ACE_ROOT}/TAO/orbsvcs/tests/Notify/lib");
$consumer->AddLibPath("$ENV{ACE_ROOT}/TAO/orbsvcs/tests/Notify/lib");

my $naming_ior = "naming.ior";
my $consumer_ior = "consumer.ior";
my $supplier_conf = "supplier.conf";
my $consumer_conf = "consumer.conf";

my $supplier_spconffile = $supplier->LocalFile ($supplier_conf);
my $consumer_cnconffile = $consumer->LocalFile ($consumer_conf);

my $nm_service_nmiorfile = $nm_service->LocalFile ($naming_ior);
my $supplier_nmiorfile = $supplier->LocalFile ($naming_ior);
my $supplier_cniorfile = $supplier->LocalFile ($consumer_ior);
my $consumer_nmiorfile = $consumer->LocalFile ($naming_ior);
my $consumer_cniorfile = $consumer->LocalFile ($consumer_ior);

$nm_service->DeleteFile ($naming_ior);
$supplier->DeleteFile ($naming_ior);
$supplier->DeleteFile ($consumer_ior);
$consumer->DeleteFile ($naming_ior);
$consumer->DeleteFile ($consumer_ior);

$NM = $nm_service->CreateProcess ("../../../../../../Naming_Service/tao_cosnaming",
                              "-ORBdebuglevel $debug_level -o $nm_service_nmiorfile");

$SP = $supplier->CreateProcess ("../../../../Driver/Notify_Tests_Driver",
                                "-ORBdebuglevel $debug_level ".
                                "-ORBInitRef NameService=file://$supplier_nmiorfile ".
                                "-IORinput file://$supplier_cniorfile ".
                                "-ORBSvcConf $supplier_spconffile");

$CN = $consumer->CreateProcess ("../../../../Driver/Notify_Tests_Driver",
                                "-ORBdebuglevel $debug_level ".
                                "-ORBInitRef NameService=file://$consumer_nmiorfile ".
                                "-IORoutput $consumer_cniorfile ".
                                "-ORBSvcConf $consumer_cnconffile");

$process_status = $NM->Spawn ();
if ($process_status != 0) {
    print STDERR "ERROR: naming returned $process_status\n";
    exit 1;
}

if ($nm_service->WaitForFileTimed ($naming_ior,
                                   $nm_service->ProcessStartWaitInterval() + 585) == -1) {
    print STDERR "ERROR: cannot find file <$nm_service_nmiorfile>\n";
    $NM->Kill (); $NM->TimedWait (1);
    exit 1;
}

if ($nm_service->GetFile ($naming_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$nm_service_nmiorfile>\n";
    $NM->Kill (); $NM->TimedWait (1);
    exit 1;
}

if ($supplier->PutFile ($naming_ior) == -1) {
    print STDERR "ERROR: cannot set file <$supplier_nmiorfile>\n";
    $NM->Kill (); $NM->TimedWait (1);
    exit 1;
}

if ($consumer->PutFile ($naming_ior) == -1) {
    print STDERR "ERROR: cannot set file <$consumer_nmiorfile>\n";
    $NM->Kill (); $NM->TimedWait (1);
    exit 1;
}

print STDERR "Running Consumer with arguments: ".$CN->Arguments ()."\n";
$process_status = $CN->Spawn ();
if ($process_status != 0) {
    print STDERR "ERROR: consumer returned $process_status\n";
    exit 1;
}

if ($consumer->WaitForFileTimed ($consumer_ior,
                                 $consumer->ProcessStartWaitInterval() + 585) == -1) {
    print STDERR "ERROR: cannot find file <$consumer_cniorfile>\n";
    $CN->Kill (); $CN->TimedWait (1);
    $NM->Kill (); $NM->TimedWait (1);
    exit 1;
}

if ($consumer->GetFile ($consumer_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$consumer_cniorfile>\n";
    $CN->Kill (); $CN->TimedWait (1);
    $NM->Kill (); $NM->TimedWait (1);
    exit 1;
}

if ($supplier->PutFile ($consumer_ior) == -1) {
    print STDERR "ERROR: cannot set file <$supplier_cniorfile>\n";
    $CN->Kill (); $CN->TimedWait (1);
    $NM->Kill (); $NM->TimedWait (1);
    exit 1;
}

print STDERR "Running Supplier with arguments: ".$SP->Arguments ()."\n";
$process_status = $SP->SpawnWaitKill ($supplier->ProcessStartWaitInterval() + 585);

if ($process_status != 0) {
    print STDERR "ERROR: supplier returned $process_status\n";
    $CN->Kill (); $CN->TimedWait (1);
    $NM->Kill (); $NM->TimedWait (1);
    exit 1;
}

$process_status = $CN->WaitKill ($consumer->ProcessStopWaitInterval());
if ($process_status != 0) {
    print STDERR "ERROR: consumer returned $process_status\n";
    $status = 1;
}

$NM->Kill ();

$nm_service->DeleteFile ($naming_ior);
$supplier->DeleteFile ($naming_ior);
$supplier->DeleteFile ($consumer_ior);
$consumer->DeleteFile ($naming_ior);
$consumer->DeleteFile ($consumer_ior);

if ($#ARGV > -1) {
    $results_directory = $ARGV[0];
    print STDERR "Saving results to $results_directory\n";

    mkdir $results_directory, 0777;

    @list=glob("*.dat");
    for $file (@list) {
        copy ("$file", "$results_directory/$file");
    }

    @list=glob("*.conf");
    for $file (@list) {
        copy ("$file", "$results_directory/$file");
    }
}

exit $status;
