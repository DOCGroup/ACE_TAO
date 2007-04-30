eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

$SV = new PerlACE::Process ("server", "-ORBUseIMR 1 -ORBDefaultInitRef corbaloc:iiop:localhost:12345");

$server = $SV->SpawnWaitKill (30);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

exit $status;
