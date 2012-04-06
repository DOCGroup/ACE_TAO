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

my $nm_service = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $collocated = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
$collocated->AddLibPath("$ENV{ACE_ROOT}/TAO/orbsvcs/tests/Notify/lib");

my $iorbase = "naming.ior";
my $collocated_conf = "colocated.conf";

my $collocated_conffile = $collocated->LocalFile($collocated_conf);
my $nm_service_iorfile = $nm_service->LocalFile($iorbase);
my $collocated_iorfile = $collocated->LocalFile($iorbase);
$nm_service->DeleteFile($iorbase);
$collocated->DeleteFile($iorbase);

$NM_SV = $nm_service->CreateProcess ("../../../../../../Naming_Service/tao_cosnaming",
                                     "-ORBdebuglevel $debug_level -o $nm_service_iorfile");

$CL = $collocated->CreateProcess ("../../../../Driver/Notify_Tests_Driver",
                                  "-ORBInitRef NameService=file://$collocated_iorfile ".
                                  "-ORBSvcConf $collocated_conffile -Skip_Priority_Levels_Check");

$process_status = $NM_SV->Spawn ();

if ($process_status != 0) {
    print STDERR "ERROR: Naming service returned $process_status\n";
    exit 1;
}

if ($nm_service->WaitForFileTimed ($iorbase,
                               $nm_service->ProcessStartWaitInterval() + 45) == -1) {
    print STDERR "ERROR: cannot find file <$nm_service_iorfile>\n";
    $NM_SV->Kill (); $NM_SV->TimedWait (1);
    exit 1;
}

if ($nm_service->GetFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$nm_service_iorfile>\n";
    $NM_SV->Kill (); $NM_SV->TimedWait (1);
    exit 1;
}

if ($collocated->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$collocated_iorfile>\n";
    $NM_SV->Kill (); $NM_SV->TimedWait (1);
    exit 1;
}

$process_status = $CL->SpawnWaitKill ($collocated->ProcessStartWaitInterval() + 585);

if ($process_status != 0) {
    print STDERR "ERROR: collocated returned $process_status\n";
    $status = 1;
}

$NM_SV->Kill ();

$nm_service->DeleteFile($iorbase);
$collocated->DeleteFile($iorbase);

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
