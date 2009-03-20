eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

$SV = new PerlACE::Process ("Collocated_Test");

$server = $SV->SpawnWaitKill (60);

if ($server != 0) {
    print STDERR "ERROR: Collocated_Test returned $server \n";
    $status = 1;
}

exit $status;
