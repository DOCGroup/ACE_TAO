eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("oc.ior");
$status = 0;

$CL_ALT_IIOP = new PerlACE::Process ("client", " -k file://$iorfile " .
                                     "-orbsvcconf oc_svc.conf");

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    exit 1;
}

$client = $CL_ALT_IIOP->SpawnWaitKill (30);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

exit $status;
