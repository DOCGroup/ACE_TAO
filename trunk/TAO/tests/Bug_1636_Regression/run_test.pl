eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs the federated Name Service timeout test.
# It starts all the servers and clients as necessary.

use lib "$ENV{ACE_ROOT}/bin";

use PerlACE::Run_Test;

$status = 0;

$CLIENT = new PerlACE::Process ("client");

$client = $CLIENT->SpawnWaitKill (30);

if ($client !=0 ) {
    print STDERR "ERROR: client returned $client\n";
}

exit $status;
