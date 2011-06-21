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

my $ifr_service = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $tao_ifr = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

# The location of the implementation repository binaries
my $ifr_bin_path = "../../IFR_Service";

# The location of the tao_ifr IFR utility
my $tao_ifr_bin_path = "$ENV{ACE_ROOT}/bin";

# IOR file
my $ior_file = "ifr.ior";

# IDL File
my $idl_file = "test.idl";

#Log file
my @result_files = ();
for ($i = 0; $i < 2; $i++) {
    $result_files[$i] = "test_result_$i.log";
}

#Files specification
$tao_ifr_idlfile = $tao_ifr->LocalFile($idl_file);
$tao_ifr_iorfile = $tao_ifr->LocalFile($ior_file);

$ifr_service_iorfile = $ifr_service->LocalFile($ior_file);

$tao_ifr->DeleteFile($ior_file);
$ifr_service->DeleteFile($ior_file);

$ifr_service_result_file = $ifr_service->LocalFile ($result_files[0]);
$tao_ifr_result_file     = $ifr_service->LocalFile ($result_files[1]);
$ifr_service->DeleteFile ($result_files[0]);
$tao_ifr->DeleteFile ($result_files[1]);

$IFRSERVICE = $ifr_service->CreateProcess("$ifr_bin_path/tao_ifr_service", "-ORBdebuglevel $debug_level ".
                                         "-ORBLogFile $ifr_service_result_file ".
                                         "-o $ifr_service_iorfile");
$TAOIFR = $tao_ifr->CreateProcess("$tao_ifr_bin_path/tao_ifr", "-ORBLogFile $tao_ifr_result_file ".
                                         "-ORBInitRef InterfaceRepository=file://$tao_ifr_iorfile ".
                                         "-Cw $tao_ifr_idlfile");

sub analyse_results {
    my $result_file = $_[0];

    if (! -r $result_file) {
        print STDERR "ERROR: cannot find $result_file\n";
        return 1;
    }

    my $match = 0;
    open (FILE, $result_file) or return -1;
    while (<FILE>) {
        $match = /Warning - identifier spellings differ only in case:/;
        last if $match;
    }
    close FILE;

    return $match ? 0 : -1;
}

sub test_body {
    # Start the IFR Service to generate an IOR file for the tao_ifr to use...
    $ifr_status = $IFRSERVICE->Spawn ();

    if ($ifr_status != 0) {
        print STDERR "ERROR: ifr_service Spawn returned $ifr_status\n";
        return 1;
    }

    if ($ifr_service->WaitForFileTimed ($ior_file,
                               $ifr_service->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$ifr_service_iorfile>\n";
        $IFRSERVICE->Kill (); $IFRSERVICE->TimedWait (1);
        return 1;
    }

    if ($ifr_service->GetFile ($ior_file) == -1) {
        print STDERR "ERROR: cannot retrieve file <$ifr_service_iorfile>\n";
        $IFRSERVICE->Kill (); $IFRSERVICE->TimedWait (1);
        return 1;
    }

    if ($tao_ifr->PutFile ($ior_file) == -1) {
        print STDERR "ERROR: cannot set file <$tao_ifr_iorfile>\n";
        $IFRSERVICE->Kill (); $IFRSERVICE->TimedWait (1);
        return 1;
    }

    $tao_status = $TAOIFR->SpawnWaitKill ($tao_ifr->ProcessStartWaitInterval() + 15);
    if ($tao_status != 0) {
        print STDERR "ERROR: tao_ifr returned $tao_status\n";
        $status = 1;
    }

    $match = 0;
#    $match = analyse_results ($ifr_service_result_file); $ # this string is not currently needed
    $match += analyse_results ($tao_ifr_result_file);

    # Tidy up
    $ifr_status = $IFRSERVICE->TerminateWaitKill ($ifr_service->ProcessStopWaitInterval());
    if ($ifr_status != 0) {
        print STDERR "ERROR: ifr_service TerminateWaitKill returned $ifr_status\n";
        return 1;
    }

    return $match;
}

# Run regression for bug #1436
$status = test_body();

if ($status != 0) {
    print STDERR "ERROR: Regression test for Bug #1436 failed\n";
}
else {
    print "Regression test for Bug #1436 passed.\n";
}

$tao_ifr->DeleteFile($ior_file);
$ifr_service->DeleteFile($ior_file);
$ifr_service->DeleteFile ($result_files[0]);
$tao_ifr->DeleteFile ($result_files[1]);

exit $status;
