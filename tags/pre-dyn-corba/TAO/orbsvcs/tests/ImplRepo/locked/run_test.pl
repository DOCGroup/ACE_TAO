eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../bin";
use PerlACE::Run_Test;

################################################################################
# Program locations

$imr_locator_ior = PerlACE::LocalFile ("imr_locator.ior");
$imr_activator_ior = PerlACE::LocalFile ("imr_activator.ior");
$pfile = PerlACE::LocalFile ("persistence.dat");

$IMR_LOCATOR = new PerlACE::Process ("../../../ImplRepo_Service/ImplRepo_Service");
$IMR_ACTIVATOR = new PerlACE::Process ("../../../ImplRepo_Service/ImR_Activator");

if ($^O eq "MSWin32") {
    $TAO_IMR = new PerlACE::Process ("../../../../../bin/tao_imr");
}
else {
    $TAO_IMR = new PerlACE::Process ("../../../ImplRepo_Service/tao_imr");
}

################################################################################

$errors = 0;

unlink $imr_locator_ior;
unlink $imr_activator_ior;
unlink $pfile;

################################################################################
## Start the implementation Repository Locator

$IMR_LOCATOR->Arguments ("-o $imr_locator_ior -d ");
$IMR_LOCATOR->Spawn ();

if (PerlACE::waitforfile_timed ($imr_locator_ior, 10) == -1) {
    print STDERR "ERROR: waiting for $imr_locator_ior\n";
    $IMR_LOCATOR->Kill ();
    exit 1;
}

## Start the implementation Repository Activator

$IMR_ACTIVATOR->Arguments ("-o $imr_activator_ior -d 0 -p $pfile -ORBInitRef ImplRepoService=file://$imr_locator_ior");
$IMR_ACTIVATOR->Spawn ();

if (PerlACE::waitforfile_timed ($imr_activator_ior, 10) == -1) {
    print STDERR "ERROR: waiting for $imr_activator_ior\n";
    $IMR_ACTIVATOR->Kill ();
    $IMR_LOCATOR->Kill ();
    exit 1;
}

################################################################################
## Test out commands on the IMR

print "===== Adding a server\n";

$TAO_IMR->Arguments("-ORBInitRef ImplRepoService=file://$imr_locator_ior"
		    . " add Foo -c foobarbaz");

$taoimr = $TAO_IMR->SpawnWaitKill (60);

if ($taoimr != 0) {
    print STDERR "ERROR: tao_imr (add) returned $taoimr\n";
    ++$errors;
}

print "===== Updating a server\n";

$TAO_IMR->Arguments("-ORBInitRef ImplRepoService=file://$imr_locator_ior"
                    . " update Foo -w foodir");

$taoimr = $TAO_IMR->SpawnWaitKill (60);

if ($taoimr != 0) {
    print STDERR "ERROR: tao_imr (update) returned $taoimr\n";
    ++$errors;
}

print "===== Removing a server\n";

$TAO_IMR->Arguments("-ORBInitRef ImplRepoService=file://$imr_locator_ior"
                    . " remove Foo");

$taoimr = $TAO_IMR->SpawnWaitKill (60);

if ($taoimr != 0) {
    print STDERR "ERROR: tao_imr (remove) returned $taoimr\n";
    ++$errors;
}

print "===== Readding a server\n";

$TAO_IMR->Arguments("-ORBInitRef ImplRepoService=file://$imr_locator_ior"
                    . " add Foo -c foobarbaz");

$taoimr = $TAO_IMR->SpawnWaitKill (60);

if ($taoimr != 0) {
    print STDERR "ERROR: tao_imr (add) returned $taoimr\n";
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

$iserver = $IMR_ACTIVATOR->TerminateWaitKill (5);

if ($iserver != 0) {
    print STDERR "ERROR: IMR returned $iserver\n";
    ++$errors;
}

unlink $imr_activator_ior;

################################################################################
## Restart the Implementation Repository in locked mode.

$IMR_LOCATOR->Arguments ("-o $imr_locator_ior -d ");
$IMR_LOCATOR->Spawn ();

if (PerlACE::waitforfile_timed ($imr_locator_ior, 10) == -1) {
    print STDERR "ERROR: waiting for $imr_locator_ior\n";
    $IMR_LOCATOR->Kill ();
    exit 1;
}

$IMR_ACTIVATOR->Arguments ("-o $imr_activator_ior -d 0 -l -p $pfile -ORBInitRef ImplRepoService=file://$imr_locator_ior");
$IMR_ACTIVATOR->Spawn ();

if (PerlACE::waitforfile_timed ($imr_activator_ior, 10) == -1) {
    print STDERR "ERROR: waiting for $imr_activator_ior\n";
    $IMR_ACTIVATOR->Kill ();
    $IMR_LOCATOR->Kill ();
    exit 1;
}

################################################################################
## Test out commands on the IMR

print "===== Adding a server (should fail)\n";

$TAO_IMR->Arguments("-ORBInitRef ImplRepoService=file://$imr_locator_ior"
                    . " add Foo2 -c foobarbaz");

$taoimr = $TAO_IMR->SpawnWaitKill (60);

if ($taoimr != 2) {
    print STDERR "ERROR: tao_imr (add) returned $taoimr\n";
    ++$errors;
}

print "===== Updating a server (should fail)\n";

$TAO_IMR->Arguments("-ORBInitRef ImplRepoService=file://$imr_locator_ior"
                    . " update Foo -w foodir");

$taoimr = $TAO_IMR->SpawnWaitKill (60);

if ($taoimr != 2) {
    print STDERR "ERROR: tao_imr (update) returned $taoimr\n";
    ++$errors;
}

print "===== Removing a server (should fail)\n";

$TAO_IMR->Arguments("-ORBInitRef ImplRepoService=file://$imr_locator_ior"
                    . " remove Foo");

$taoimr = $TAO_IMR->SpawnWaitKill (60);

if ($taoimr != 2) {
    print STDERR "ERROR: tao_imr (remove) returned $taoimr\n";
    ++$errors;
}

################################################################################
## Kill the IMR_LOCATOR and IMR_ACTIVATOR

$iserver = $IMR_LOCATOR->TerminateWaitKill (5);

if ($iserver != 0) {
    print STDERR "ERROR: IMR returned $iserver\n";
    ++$errors;
}

unlink $imr_locator_ior;

$iserver = $IMR_ACTIVATOR->TerminateWaitKill (5);

if ($iserver != 0) {
    print STDERR "ERROR: IMR returned $iserver\n";
    ++$errors;
}

unlink $imr_activator_ior;
unlink $pfile;

exit $errors;
