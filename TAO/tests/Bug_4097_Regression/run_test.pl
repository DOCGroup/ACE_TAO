eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use strict;

my $target = PerlACE::TestTarget::create_target(1)
    or die "Create target failed\n";
my $test = $target->CreateProcess("Bug_4097_Regression");
my $result = $test->SpawnWaitKill($target->ProcessStartWaitInterval());
if ($result != 0) {
    print STDERR "ERROR: test returned $result\n";
    exit 1;
}
exit 0;
