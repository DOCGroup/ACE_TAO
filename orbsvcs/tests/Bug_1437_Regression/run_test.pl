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

my $locator   = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $activator = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $tao_imr   = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

# The location of the implementation repository binaries
my $imr_bin_path  = "";

my $imr_ior        = "impl.ior";
my $activator_ior  = "activator.ior";

my $locator_imr_iorfile   = $locator->LocalFile ($imr_ior);
my $activator_iorfile     = $activator->LocalFile ($activator_ior);
my $activator_imr_iorfile = $activator->LocalFile ($imr_ior);
my $tao_imr_imr_iorfile   = $tao_imr->LocalFile ($imr_ior);
$locator->DeleteFile ($imr_ior);
$activator->DeleteFile ($activator_ior);
$activator->DeleteFile ($imr_ior);
$tao_imr->DeleteFile ($imr_ior);

# Run the IMR locator on a fixed port
my $locator_port = $locator->RandomPort();

$LC = $locator->CreateProcess ("../../ImplRepo_Service/tao_imr_locator",
                               "-ORBdebuglevel $debug_level -o $locator_imr_iorfile ".
                               "-ORBEndpoint iiop://:$locator_port");

$AC = $activator->CreateProcess ("../../ImplRepo_Service/tao_imr_activator",
                                 "-ORBdebuglevel $debug_level -o $activator_iorfile ".
                                 "-ORBInitRef ImplRepoService=file://$activator_imr_iorfile");

$TI = $tao_imr->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_imr",
                               "-ORBInitRef ImplRepoService=file://$tao_imr_imr_iorfile add \"\"");

sub test_body {

    $process_status = $LC->Spawn ();

    if ($process_status != 0) {
        print STDERR "ERROR: locator returned $process_status\n";
        return 1;
    }

    if ($locator->WaitForFileTimed ($imr_ior,
                                    $locator->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$locator_imr_iorfile>\n";
        $LC->Kill (); $LC->TimedWait (1);
        return 1;
    }

    if ($locator->GetFile ($imr_ior) == -1) {
        print STDERR "ERROR: cannot retrieve file <$locator_imr_iorfile>\n";
        $LC->Kill (); $LC->TimedWait (1);
        return 1;
    }

    if ($activator->PutFile ($imr_ior) == -1) {
        print STDERR "ERROR: cannot set file <$activator_imr_iorfile>\n";
        $LC->Kill (); $LC->TimedWait (1);
        return 1;
    }

    if ($tao_imr->PutFile ($imr_ior) == -1) {
        print STDERR "ERROR: cannot set file <$tao_imr_imr_iorfile>\n";
        $LC->Kill (); $LC->TimedWait (1);
        return 1;
    }

    $process_status = $AC->Spawn ();

    if ($process_status != 0) {
        print STDERR "ERROR: activator returned $process_status\n";
        $LC->Kill (); $LC->TimedWait (1);
        return 1;
    }

    if ($activator->WaitForFileTimed ($activator_ior,
                                      $activator->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$activator_iorfile>\n";
        $AC->Kill (); $AC->TimedWait (1);
        $LC->Kill (); $LC->TimedWait (1);
        return 1;
    }

    # Add the illegal persistent POA name to the IMR
    $result = $TI->SpawnWaitKill ($tao_imr->ProcessStartWaitInterval() + 15);

    # If the add of an empty string failed, then
    # the test of the tao_imr succeeded.
    if ($result != 0) {
        $result = 0;
    }
    else {
        $result = -1;
    }

   # Tidy up
   $AC->TerminateWaitKill ($activator->ProcessStopWaitInterval());
   $LC->TerminateWaitKill ($locator->ProcessStopWaitInterval());
   return $result;
}

# Run regression for bug #1437
$status = test_body();

if ($status != 0) {
   print STDERR "ERROR: Regression test for Bug #1437 failed\n";
}
else {
   print "Regression test for Bug #1437 passed.\n";
}

$locator->DeleteFile ($imr_ior);
$activator->DeleteFile ($activator_ior);
$activator->DeleteFile ($imr_ior);
$tao_imr->DeleteFile ($imr_ior);

exit $status;
