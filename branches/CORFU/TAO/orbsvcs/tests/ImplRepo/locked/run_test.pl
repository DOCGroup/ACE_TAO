eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

################################################################################
# Program locations

$imr_locator_ior = PerlACE::LocalFile ("imr_locator.ior");
$pfile = PerlACE::LocalFile ("persistence.dat");

$IMR_LOCATOR = new PerlACE::Process ("../../../ImplRepo_Service/ImplRepo_Service");
$TAO_IMR = new PerlACE::Process ("$ENV{ACE_ROOT}/bin/tao_imr");

# We want the tao_imr executable to be found exactly in the path
# given, without being modified by the value of -ExeSubDir.
# So, we tell its Process object to ignore the setting of -ExeSubDir.

$TAO_IMR->IgnoreExeSubDir (1);

################################################################################

$errors = 0;

unlink $imr_locator_ior;
unlink $pfile;

################################################################################
## Start the implementation Repository Locator

$IMR_LOCATOR->Arguments ("-p $pfile -o $imr_locator_ior -d 2");
$IMR_LOCATOR->Spawn ();

if (PerlACE::waitforfile_timed ($imr_locator_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: waiting for $imr_locator_ior\n";
    $IMR_LOCATOR->Kill ();
    exit 1;
}

################################################################################
## Test out commands on the IMR

print "===== Adding a server\n";

$TAO_IMR->Arguments("-ORBInitRef ImplRepoService=file://$imr_locator_ior"
		    . " add Foo -c foobarbaz");

$taoimr = $TAO_IMR->SpawnWaitKill (10);

if ($taoimr != 0) {
    print STDERR "ERROR: tao_imr (add) returned $taoimr\n";
    ++$errors;
}

print "===== Updating a server\n";

$TAO_IMR->Arguments("-ORBInitRef ImplRepoService=file://$imr_locator_ior"
                    . " update Foo -w foodir" );

$taoimr = $TAO_IMR->SpawnWaitKill (10);

if ($taoimr != 0) {
    print STDERR "ERROR: tao_imr (update) returned $taoimr\n";
    ++$errors;
}

print "===== Removing a server\n";

$TAO_IMR->Arguments("-ORBInitRef ImplRepoService=file://$imr_locator_ior"
                    . " remove Foo");

$taoimr = $TAO_IMR->SpawnWaitKill (10);

if ($taoimr != 0) {
    print STDERR "ERROR: tao_imr (remove) returned $taoimr\n";
    ++$errors;
}

print "===== Re-adding a server\n";

$TAO_IMR->Arguments("-ORBInitRef ImplRepoService=file://$imr_locator_ior"
                    . " add Foo -c foobarbaz");

$taoimr = $TAO_IMR->SpawnWaitKill (10);

if ($taoimr != 0) {
    print STDERR "ERROR: tao_imr (add) returned $taoimr\n";
    ++$errors;
}

################################################################################
## Kill the IMR
print "===== Killing the ImR and restarting in locked mode.\n";

$iserver = $IMR_LOCATOR->TerminateWaitKill (5);

if ($iserver != 0) {
    print STDERR "ERROR: IMR returned $iserver\n";
    ++$errors;
}

unlink $imr_locator_ior;

################################################################################
## Restart the Implementation Repository in locked mode.

$IMR_LOCATOR->Arguments ("-l -p $pfile -o $imr_locator_ior -d 2");
$IMR_LOCATOR->Spawn ();

if (PerlACE::waitforfile_timed ($imr_locator_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: waiting for $imr_locator_ior\n";
    $IMR_LOCATOR->Kill ();
    exit 1;
}

################################################################################
## Test out commands on the IMR

print "===== Listing registered servers.\n";

$TAO_IMR->Arguments("-ORBInitRef ImplRepoService=file://$imr_locator_ior"
                    . " list");

$taoimr = $TAO_IMR->SpawnWaitKill (10);

if ($taoimr != 0) {
    print STDERR "ERROR: tao_imr (list) returned $taoimr\n";
    ++$errors;
}

print "===== Adding a server (should fail)\n";

$TAO_IMR->Arguments("-ORBInitRef ImplRepoService=file://$imr_locator_ior"
                    . " add Foo2 -c foobarbaz");

$taoimr = $TAO_IMR->SpawnWaitKill (10);

## Note : If you receive a 5 (NOT_FOUND) then it's likely that
## persistence isn't working correctly.
if ($taoimr != 2) {  # NO_PERMISSION
    print STDERR "ERROR: tao_imr (add) returned $taoimr\n";
    ++$errors;
}

print "===== Updating a server (should fail)\n";

$TAO_IMR->Arguments("-ORBInitRef ImplRepoService=file://$imr_locator_ior"
                    . " update Foo -w foodir");

$taoimr = $TAO_IMR->SpawnWaitKill (10);

if ($taoimr != 2) {  # NO_PERMISSION
    print STDERR "ERROR: tao_imr (update) returned $taoimr\n";
    ++$errors;
}

print "===== Removing a server (should fail)\n";

$TAO_IMR->Arguments("-ORBInitRef ImplRepoService=file://$imr_locator_ior"
                    . " remove Foo");

$taoimr = $TAO_IMR->SpawnWaitKill (10);

if ($taoimr != 2) {  # NO_PERMISSION
    print STDERR "ERROR: tao_imr (remove) returned $taoimr\n";
    ++$errors;
}

################################################################################
## Kill the IMR

$iserver = $IMR_LOCATOR->TerminateWaitKill (5);

if ($iserver != 0) {
    print STDERR "ERROR: IMR returned $iserver\n";
    ++$errors;
}

unlink $imr_locator_ior;

unlink $pfile;

exit $errors;
