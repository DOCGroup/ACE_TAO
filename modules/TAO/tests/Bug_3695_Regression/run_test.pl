eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $target = PerlACE::TestTarget::create_target(1) || die "Create target 1 failed\n";

$status = 0;

$SV = $target->CreateProcess ("Bug_3695_Regression");

$server = $SV->SpawnWaitKill ($target->ProcessStartWaitInterval());

if ($server != 0) {
    print STDERR "ERROR: Bug_3695_Regression returned $server\n";
    $status = 1;
}

$target->GetStderrLog();

exit $status;
