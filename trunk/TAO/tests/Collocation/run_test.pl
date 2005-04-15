eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;

$status = 0;

print STDOUT "Collocation\n\n";

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("Collocation");
}
else {
    $SV = new PerlACE::Process ("Collocation");
}


$server = $SV->SpawnWaitKill (60);

if ($server != 0) {
    print STDERR "ERROR: Collocation returned $server \n";
    $status = 1;
}

exit $status;
