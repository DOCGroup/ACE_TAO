eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

################################################################################
# Program locations

$imr_ior = PerlACE::LocalFile ("imr.ior");
$name_ior = PerlACE::LocalFile ("name.ior");
$activator_ior = PerlACE::LocalFile("activator.ior");

$IMR = new PerlACE::Process ("../../../ImplRepo_Service/ImplRepo_Service");
$ACTIVATOR = new PerlACE::Process("../../../ImplRepo_Service/ImR_Activator");
$NS  = new PerlACE::Process ("../../../Naming_Service/Naming_Service");
$TAO_IMR = new PerlACE::Process ("$ENV{ACE_ROOT}/bin/tao_imr");

# We want the tao_imr executable to be found exactly in the path
# given, without being modified by the value of -ExeSubDir.
# So, we tell its Process object to ignore the setting of -ExeSubDir.

$TAO_IMR->IgnoreExeSubDir (1);

$TEST = new PerlACE::Process ("test");

$imr_init_ref = "-ORBInitRef ImplRepoService=file://$imr_ior";

################################################################################

$errors = 0;

unlink $imr_ior;
unlink $name_ior;
unlink $activator_ior;

################################################################################
## Start the implementation Repository

$IMR->Arguments ("-o $imr_ior -d 2");
$IMR->Spawn ();

if (PerlACE::waitforfile_timed ($imr_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: waiting for $imr_ior\n";
    $IMR->Kill ();
    exit 1;
}

$ACTIVATOR->Arguments("-d 1 -o $activator_ior $imr_init_ref");
$ACTIVATOR->Spawn();

if (PerlACE::waitforfile_timed ($activator_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: waiting for $activator_ior\n";
    $IMR->Kill ();
    $ACTIVATOR->Kill();
    exit 1;
}

################################################################################
## Register the NameService

$TAO_IMR->Arguments("$imr_init_ref"
                    . " add NameService "
                    ." -c \"" . $NS->Executable ()
                       ." $imr_init_ref"
                       ." -ORBUseIMR 1 .\"");

$taoimr = $TAO_IMR->SpawnWaitKill (60);

if ($taoimr != 0) {
    print STDERR "ERROR: tao_imr (add) returned $taoimr\n";
    ++$errors;
}

################################################################################
## Create IOR for NameService

$TAO_IMR->Arguments ("$imr_init_ref ior NameService -f $name_ior");


$taoimr = $TAO_IMR->SpawnWaitKill (60);

if ($taoimr != 0) {
    print STDERR "ERROR: tao_imr (ior) returned $taoimr\n";
    ++$errors;
}

################################################################################
## Run the test

$TEST->Arguments ("-ORBInitRef NameService=file://$name_ior");

$test = $TEST->SpawnWaitKill (60);

if ($test != 0) {
    print STDERR "ERROR: test returned $test\n";
    ++$errors;
}


################################################################################
## Shutdown the NameService

$TAO_IMR->Arguments ("-ORBInitRef ImplRepoService=file://$imr_ior shutdown "
                     . "NameService ");

$taoimr = $TAO_IMR->SpawnWaitKill (60);

if ($taoimr != 0) {
    print STDERR "ERROR: tao_imr (shutdown) returned $taoimr\n";
    ++$errors;
}

################################################################################
## Kill the IMR

$iserver = $ACTIVATOR->TerminateWaitKill (5);

if ($iserver != 0) {
    print STDERR "ERROR: ImR_Activator returned $iserver\n";
    ++$errors;
}

$iserver = $IMR->TerminateWaitKill (5);

if ($iserver != 0) {
    print STDERR "ERROR: IMR returned $iserver\n";
    ++$errors;
}

unlink $imr_ior;
unlink $name_ior;
unlink $activator_ior;

exit $errors;
