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
$pfile = PerlACE::LocalFile ("persistence.dat");

$IMR = new PerlACE::Process ("../../../ImplRepo_Service/ImplRepo_Service");

if ($^O eq "MSWin32") {
    $TAO_IMR = new PerlACE::Process ("../../../../../bin/tao_imr");
}
else {
    $TAO_IMR = new PerlACE::Process ("../../../ImplRepo_Service/tao_imr");
}

################################################################################

$errors = 0;

unlink $imr_ior;
unlink $pfile;

################################################################################
## Start the implementation Repository 

$IMR->Arguments ("-o $imr_ior -d 0 -p $pfile");
$IMR->Spawn ();

if (PerlACE::waitforfile_timed ($imr_ior, 10) == -1) {
    print STDERR "ERROR: waiting for $imr_ior\n";
    $IMR->Kill ();
    exit 1;
}

################################################################################
## Test out commands on the IMR

print "===== Adding a server\n";

$TAO_IMR->Arguments("-ORBInitRef ImplRepoService=file://$imr_ior"
                    . " add Foo -c foobarbaz");

$taoimr = $TAO_IMR->SpawnWaitKill (60);

if ($taoimr != 0) {
    print STDERR "ERROR: tao_imr (add) returned $taoimr\n";
    ++$errors;
}

print "===== Updating a server\n";

$TAO_IMR->Arguments("-ORBInitRef ImplRepoService=file://$imr_ior"
                    . " update Foo -w foodir");

$taoimr = $TAO_IMR->SpawnWaitKill (60);

if ($taoimr != 0) {
    print STDERR "ERROR: tao_imr (update) returned $taoimr\n";
    ++$errors;
}

print "===== Removing a server\n";

$TAO_IMR->Arguments("-ORBInitRef ImplRepoService=file://$imr_ior"
                    . " remove Foo");

$taoimr = $TAO_IMR->SpawnWaitKill (60);

if ($taoimr != 0) {
    print STDERR "ERROR: tao_imr (remove) returned $taoimr\n";
    ++$errors;
}

print "===== Readding a server\n";

$TAO_IMR->Arguments("-ORBInitRef ImplRepoService=file://$imr_ior"
                    . " add Foo -c foobarbaz");

$taoimr = $TAO_IMR->SpawnWaitKill (60);

if ($taoimr != 0) {
    print STDERR "ERROR: tao_imr (add) returned $taoimr\n";
    ++$errors;
}

################################################################################
## Kill the IMR

$iserver = $IMR->TerminateWaitKill (5); 

if ($iserver != 0) {
    print STDERR "ERROR: IMR returned $iserver\n";
    ++$errors;
}

unlink $imr_ior;

################################################################################
## Restart the Implementation Repository in locked mode.

$IMR->Arguments ("-o $imr_ior -d 0 -l -p $pfile");
$IMR->Spawn ();

if (PerlACE::waitforfile_timed ($imr_ior, 10) == -1) {
    print STDERR "ERROR: waiting for $imr_ior\n";
    $IMR->Kill ();
    exit 1;
}

################################################################################
## Test out commands on the IMR

print "===== Adding a server (should fail)\n";

$TAO_IMR->Arguments("-ORBInitRef ImplRepoService=file://$imr_ior"
                    . " add Foo2 -c foobarbaz");

$taoimr = $TAO_IMR->SpawnWaitKill (60);

if ($taoimr != 2) {
    print STDERR "ERROR: tao_imr (add) returned $taoimr\n";
    ++$errors;
}

print "===== Updating a server (should fail)\n";

$TAO_IMR->Arguments("-ORBInitRef ImplRepoService=file://$imr_ior"
                    . " update Foo -w foodir");

$taoimr = $TAO_IMR->SpawnWaitKill (60);

if ($taoimr != 2) {
    print STDERR "ERROR: tao_imr (update) returned $taoimr\n";
    ++$errors;
}

print "===== Removing a server (should fail)\n";

$TAO_IMR->Arguments("-ORBInitRef ImplRepoService=file://$imr_ior"
                    . " remove Foo");

$taoimr = $TAO_IMR->SpawnWaitKill (60);

if ($taoimr != 2) {
    print STDERR "ERROR: tao_imr (remove) returned $taoimr\n";
    ++$errors;
}

################################################################################
## Kill the IMR

$iserver = $IMR->TerminateWaitKill (5); 

if ($iserver != 0) {
    print STDERR "ERROR: IMR returned $iserver\n";
    ++$errors;
}

unlink $imr_ior;
unlink $pfile;

exit $errors;
