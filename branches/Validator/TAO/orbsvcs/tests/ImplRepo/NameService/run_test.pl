eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../bin";
use PerlACE::Run_Test;

################################################################################
# Program locations

$imr_ior = PerlACE::LocalFile ("imr.ior");
$name_ior = PerlACE::LocalFile ("name.ior");

$IMR = new PerlACE::Process ("../../../ImplRepo_Service/ImplRepo_Service");
$NS  = new PerlACE::Process ("../../../Naming_Service/Naming_Service");

if ($^O eq "MSWin32") {
    $TAO_IMR = new PerlACE::Process ("../../../../../bin/tao_imr");
}
else {
    $TAO_IMR = new PerlACE::Process ("../../../ImplRepo_Service/tao_imr");
}

$TEST = new PerlACE::Process ("test");

################################################################################

$errors = 0;

unlink $imr_ior;
unlink $name_ior;

################################################################################
## Start the implementation Repository 

$IMR->Arguments ("-o $imr_ior -d 0");
$IMR->Spawn ();

if (PerlACE::waitforfile_timed ($imr_ior, 5) == -1) {
    print STDERR "ERROR: waiting for $imr_ior\n";
    $IMR->Kill ();
    exit 1;
}

################################################################################
## Register the NameService

$TAO_IMR->Arguments("-ORBInitRef ImplRepoService=file://$imr_ior"
                    . " add NameService "
                    ." -c \"" . $NS->Executable () 
                       ." -ORBInitRef ImplRepoService=file://$imr_ior"
                       ." -ORBUseIMR 1 .\"");

$taoimr = $TAO_IMR->SpawnWaitKill (60);

if ($taoimr != 0) {
    print STDERR "ERROR: tao_imr (add) returned $taoimr\n";
    ++$errors;
}

################################################################################
## Create IOR for NameService

$TAO_IMR->Arguments ("-ORBInitRef ImplRepoService=file://$imr_ior"
                     . " ior NameService -f $name_ior");


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

$iserver = $IMR->TerminateWaitKill (5); 

if ($iserver != 0) {
    print STDERR "ERROR: IMR returned $iserver\n";
    ++$errors;
}

exit $errors;
