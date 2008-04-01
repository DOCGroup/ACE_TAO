eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;  
 
# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

print "Run sever with direct collocation strategy\n";

$SV2 = new PerlACE::Process ("server", "-d -ORBCollocationStrategy direct");

$server2 = $SV2->SpawnWaitKill (10);
if ($server2 != 0) {
    print STDERR "Server with direct collocation strategy return status = $server2\n";
    exit 1;
}

print "Run sever with pass thru collocation strategy\n";

$SV1 = new PerlACE::Process ("server", "");

$server1 = $SV1->SpawnWaitKill (10);
if ($server1 != 0) {
    print STDERR "Server with thru poa collocation strategy return status = $server1\n";
    exit 1;
}

exit 0;
