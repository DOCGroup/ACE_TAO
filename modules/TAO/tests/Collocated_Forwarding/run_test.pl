eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;  
 
# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

print "Run server with direct collocation strategy\n";

if (PerlACE::is_vxworks_test()) {
  $SV2 = new PerlACE::ProcessVX ("server", "-d -ORBCollocationStrategy direct");
}
else {
  $SV2 = new PerlACE::Process ("server", "-d -ORBCollocationStrategy direct");
}

$server2 = $SV2->SpawnWaitKill (15);
if ($server2 != 0) {
    print STDERR "Server with direct collocation strategy return status = $server2\n";
    exit 1;
}

print "Run server with pass thru collocation strategy\n";

if (PerlACE::is_vxworks_test()) {
  $SV1 = new PerlACE::ProcessVX ("server", "");
}
else {
  $SV1 = new PerlACE::Process ("server", "");
}

$server1 = $SV1->SpawnWaitKill (15);
if ($server1 != 0) {
    print STDERR "Server with thru poa collocation strategy return status = $server1\n";
    exit 1;
}

exit 0;
