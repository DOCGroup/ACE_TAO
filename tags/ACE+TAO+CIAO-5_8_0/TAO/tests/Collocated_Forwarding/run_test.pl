eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

print "Run server with direct collocation strategy\n";

$SV2 = $server->CreateProcess ("server", "-d -ORBCollocationStrategy direct");

$server2 = $SV2->SpawnWaitKill ($server->ProcessStartWaitInterval());
if ($server2 != 0) {
    print STDERR "Server with direct collocation strategy return status = $server2\n";
    exit 1;
}

print "Run server with pass thru collocation strategy\n";

$SV1 = $server->CreateProcess ("server");

$server1 = $SV1->SpawnWaitKill ($server->ProcessStartWaitInterval());
if ($server1 != 0) {
    print STDERR "Server with thru poa collocation strategy return status = $server1\n";
    exit 1;
}

exit 0;
