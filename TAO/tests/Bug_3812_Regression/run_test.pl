# $Id$
# -*- perl -*-

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use strict;

my $status = 0;
my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $P = $server->CreateProcess ("Bug_3812_Regression");
print "Running Bug_3812_Regression ...\n";
my $result = $P->Spawn ();
if ($result != 0) {
    print "test FAILED\n";
    $status = 1;
}
$result = $P->WaitKill($server->ProcessStartWaitInterval());
if ($result != 0) {
    print "test FAILED\n";
    $status = 1;
}

exit $status;
