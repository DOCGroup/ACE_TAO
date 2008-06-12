eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use PerlACE::TestTarget;

PerlACE::add_lib_path ('.');

my $target = PerlACE::TestTarget::create_target ("server");

$status = 0;

print STDOUT "Collocation\n\n";

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("Collocation");
}
else {
    $SV = $target->CreateProcess ("Collocation");
}

$server = $SV->SpawnWaitKill ($target->ProcessStartWaitInterval());

if ($server != 0) {
    print STDERR "ERROR: Collocation returned $server\n";
    $status = 1;
}

$target->GetStderrLog();

exit $status;
