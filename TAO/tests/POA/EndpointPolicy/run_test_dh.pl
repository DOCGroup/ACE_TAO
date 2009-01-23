eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;

$port = 12345;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$iiopSV = $server->CreateProcess ("server", "-ORBDottedDecimalAddresses 0 -ORBUseSharedProfile 1 -h default -p $port");

print "Starting server using only IIOP\n";

$result = $iiopSV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($result != 0) {
    print STDERR "ERROR: IIOP-only server returned $result\n";
    $status = 1;
}

print "Starting server using multiple protocols\n";

$multiSV = $server->CreateProcess ("server", "-ORBDottedDecimalAddresses 0 -ORBUseSharedProfile 0 -h multi -p $port");

$result = $multiSV->SpawnWaitKill($server->ProcessStartWaitInterval());

if ($result != 0) {
    print STDERR "ERROR: multiprotocol server returned $result\n";
    $status = 1;
}

exit $status;
