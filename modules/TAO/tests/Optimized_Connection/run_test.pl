eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$baseior = "oc.ior";
$iorfile = PerlACE::LocalFile ($baseior);
$status = 0;

if (PerlACE::is_vxworks_test()) {
$CL_ALT_IIOP = new PerlACE::ProcessVX ("client", " -k file://$baseior " .
                                     "-orbsvcconf oc_svc.conf");
}
else {
$CL_ALT_IIOP = new PerlACE::Process ("client", " -k file://$iorfile " .
                                     "-orbsvcconf oc_svc.conf");
}

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
