eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

print STDOUT "Dyn Union\n\n";

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("dynuniontest");
}
else {
    $SV = new PerlACE::Process ("dynuniontest");
}


$server = $SV->SpawnWaitKill ($PerlACE::wait_interval_for_process_creation);

if ($server != 0) {
    print STDERR "ERROR: Dyn Union returned $server \n";
    $status = 1;
}

exit $status;
