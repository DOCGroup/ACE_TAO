eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

$port = 12345;

if (PerlACE::is_vxworks_test()) {
    $iiopSV = new PerlACE::ProcessVX ("server", "-ORBDottedDecimalAddresses 0 -ORBUseSharedProfile 1 -h default -p $port");
}
else {
    $iiopSV = new PerlACE::Process ("server", "-ORBDottedDecimalAddresses 0 -ORBUseSharedProfile 1 -h default -p $port");
}

if (PerlACE::is_vxworks_test()) {
    $multiSV = new PerlACE::ProcessVX ("server", "-ORBDottedDecimalAddresses 0 -ORBUseSharedProfile 0 -h multi -p $port");
}
else {
    $multiSV = new PerlACE::Process ("server", "-ORBDottedDecimalAddresses 0 -ORBUseSharedProfile 0 -h multi -p $port");
}

print "Starting server using only IIOP\n";

$result = $iiopSV->SpawnWaitKill (300);

if ($result != 0) {
    print STDERR "ERROR: IIOP-only server returned $result\n";
    $status = 1;
}

print "Starting server using multiple protocols\n";

$result = $multiSV->SpawnWaitKill(300);

if ($result != 0) {
    print STDERR "ERROR: multiprotocol server returned $result\n";
    $status = 1;
}

exit $status;
