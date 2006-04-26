eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server");
}
else {
    $SV = new PerlACE::Process ("server");
}
print STDERR "\n==== Running Register_Initial_References test ====\n";

$server = $SV->SpawnWaitKill (30);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

exit $status;
