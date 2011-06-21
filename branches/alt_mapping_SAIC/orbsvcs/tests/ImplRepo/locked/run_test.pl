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

my $imr = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $ti = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

################################################################################
# Program locations

$imriorfile = "imr_locator.ior";
$imrpfile = "persistence.dat";

my $imr_imriorfile = $imr->LocalFile ($imriorfile);
my $ti_imriorfile = $ti->LocalFile ($imriorfile);
my $imr_imrpfile = $imr->LocalFile ($imrpfile);

$imr->DeleteFile ($imriorfile);
$ti->DeleteFile ($imriorfile);
$imr->DeleteFile ($imrpfile);


$IMR = $imr->CreateProcess ("../../../ImplRepo_Service/tao_imr_locator");
$TI = $ti->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_imr");

# We want the tao_imr executable to be found exactly in the path
# given, without being modified by the value of -ExeSubDir.
# So, we tell its Process object to ignore the setting of -ExeSubDir.

$TI->IgnoreExeSubDir (1);

################################################################################

$imr->DeleteFile ($imriorfile);
$ti->DeleteFile ($imriorfile);
$imr->DeleteFile ($imrpfile);

################################################################################
## Start the implementation Repository Locator

$IMR->Arguments ("-p $imr_imrpfile -o $imr_imriorfile -d 2 -ORBDebugLevel = $debug_level");
$IMR_status = $IMR->Spawn ();
if ($IMR_status != 0) {
    print STDERR "ERROR: ImplRepo Service returned $IMR_status\n";
    exit 1;
}
if ($imr->WaitForFileTimed ($imriorfile,$imr->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$imr_imriorfile>\n";
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}
if ($imr->GetFile ($imriorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$imr_imriorfile>\n";
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}
if ($ti->PutFile ($imriorfile) == -1) {
    print STDERR "ERROR: cannot set file <$ti_imriorfile>\n";
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}

################################################################################
## Test out commands on the IMR

print "===== Adding a server\n";

$TI->Arguments("-ORBInitRef ImplRepoService=file://$ti_imriorfile ".
               " add Foo -c foobarbaz");

$TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
if ($TI_status != 0) {
    print STDERR "ERROR: tao_imr (add) returned $TI_status\n";
    $status = 1;
}

print "===== Updating a server\n";

$TI->Arguments("-ORBInitRef ImplRepoService=file://$ti_imriorfile"
                    . " update Foo -w foodir" );

$TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
if ($TI_status != 0) {
    print STDERR "ERROR: tao_imr (update) returned $TI_status\n";
    $status = 1;
}

print "===== Removing a server\n";

$TI->Arguments("-ORBInitRef ImplRepoService=file://$ti_imriorfile"
                    . " remove Foo");

$TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
if ($TI_status != 0) {
    print STDERR "ERROR: tao_imr (remove) returned $TI_status\n";
    $status = 1;
}

print "===== Re-adding a server\n";

$TI->Arguments("-ORBInitRef ImplRepoService=file://$ti_imriorfile"
                    . " add Foo -c foobarbaz");

$TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
if ($TI_status != 0) {
    print STDERR "ERROR: tao_imr (add) returned $TI_status\n";
    $status = 1;
}

################################################################################
## Kill the IMR
print "===== Killing the ImR and restarting in locked mode.\n";

$IMR_status = $IMR->TerminateWaitKill ($imr->ProcessStopWaitInterval());
if ($IMR_status != 0) {
    print STDERR "ERROR: IMR returned $IMR_status\n";
    $status = 1;
}

$imr->DeleteFile ($imriorfile);
$ti->DeleteFile ($imriorfile);

################################################################################
## Restart the Implementation Repository in locked mode.

$IMR->Arguments ("-l -p $imr_imrpfile -o $imr_imriorfile -d 2");
$IMR_status = $IMR->Spawn ();
if ($IMR_status != 0) {
    print STDERR "ERROR: ImplRepo Service returned $IMR_status\n";
    exit 1;
}
if ($imr->WaitForFileTimed ($imriorfile,$imr->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$imr_imriorfile>\n";
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}
if ($imr->GetFile ($imriorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$imr_imriorfile>\n";
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}
if ($ti->PutFile ($imriorfile) == -1) {
    print STDERR "ERROR: cannot set file <$ti_imriorfile>\n";
    $IMR->Kill (); $IMR->TimedWait (1);
    exit 1;
}

################################################################################
## Test out commands on the IMR

print "===== Listing registered servers.\n";

$TI->Arguments("-ORBInitRef ImplRepoService=file://$ti_imriorfile"
                    . " list");

$TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());
if ($TI_status != 0) {
    print STDERR "ERROR: tao_imr (list) returned $TI_status\n";
    $status = 1;
}

print "===== Adding a server (should fail)\n";

$TI->Arguments("-ORBInitRef ImplRepoService=file://$ti_imriorfile"
                    . " add Foo2 -c foobarbaz");

$TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());

## Note : If you receive a 5 (NOT_FOUND) then it's likely that
## persistence isn't working correctly.
if ($TI_status != 2) { # NO_PERMISSION
    print STDERR "ERROR: tao_imr (add) returned $TI_status\n";
    $status = 1;
}

print "===== Updating a server (should fail)\n";

$TI->Arguments("-ORBInitRef ImplRepoService=file://$ti_imriorfile"
                    . " update Foo -w foodir");

$TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());

if ($TI_status != 2) { # NO_PERMISSION
    print STDERR "ERROR: tao_imr (update) returned $TI_status\n";
    $status = 1;
}

print "===== Removing a server (should fail)\n";

$TI->Arguments("-ORBInitRef ImplRepoService=file://$ti_imriorfile"
                    . " remove Foo");

$TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());

if ($TI_status != 2) { # NO_PERMISSION
    print STDERR "ERROR: tao_imr (remove) returned $TI_status\n";
    $status = 1;
}

################################################################################
## Kill the IMR

$IMR_status = $IMR->TerminateWaitKill ($imr->ProcessStopWaitInterval());
if ($IMR_status != 0) {
    print STDERR "ERROR: IMR returned $IMR_status\n";
    $status = 1;
}

$imr->DeleteFile ($imriorfile);
$ti->DeleteFile ($imriorfile);
$imr->DeleteFile ($imrpfile);

exit $status;
