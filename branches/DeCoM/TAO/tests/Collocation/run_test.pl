eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $target = PerlACE::TestTarget::create_target(1) || die "Create target 1 failed\n";

$target->AddLibPath ('.');

$status = 0;

print STDOUT "Collocation\n\n";

$SV = $target->CreateProcess ("Collocation");

$server = $SV->SpawnWaitKill ($target->ProcessStartWaitInterval());

if ($server != 0) {
    print STDERR "ERROR: Collocation returned $server\n";
    $status = 1;
}

$target->GetStderrLog();

exit $status;
