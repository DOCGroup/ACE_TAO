eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

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

$NS->Wait();

unlink $notify_ior;

exit $ret;
