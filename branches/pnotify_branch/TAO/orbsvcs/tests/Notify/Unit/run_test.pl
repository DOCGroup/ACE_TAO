eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# ex

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$ACE_ROOT = $ENV{ACE_ROOT};
$TAO_ROOT = "$ACE_ROOT/TAO";

$notify_ior = PerlACE::LocalFile("notify.ior");

unlink $notify_ior;

$NS = new PerlACE::Process("$TAO_ROOT/orbsvcs/Notify_Service/Notify_Service");
$NS->Arguments(" -NoNameSvc -Boot -IORoutput $notify_ior -ORBEndpoint iiop://:9050 ");
$NS->Spawn();

if (PerlACE::waitforfile_timed ($notify_ior, 5) == -1) {
   print STDERR "ERROR: Timed out waiting for $notify_ior\n";
   $NS->Kill ();
   return 1;
}

## Note : For ease of use, the unit test is hardcoded to use a 
## corbaloc, which requires that the NS be started with
## -Boot and -NoNameSvc on the expected port. 
## The ior file written above is only used so that this perl script
## will know when it's safe to start the unit test.

$UTEST = new PerlACE::Process("main");
$ret = $UTEST->SpawnWaitKill(5);

$NS->Kill();

unlink $notify_ior;

exit $ret;
