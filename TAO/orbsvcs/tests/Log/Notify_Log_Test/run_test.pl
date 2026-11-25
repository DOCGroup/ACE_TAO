eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

use strict;
use warnings;
use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $target = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $missing_dir = $target->LocalFile ("missing_notify_log_dir_$$");
my $server = $target->CreateProcess (
  "$ENV{TAO_ROOT}/orbsvcs/Logging_Service/Notify_Logging_Service/tao_tls_notify",
  "-x -o $missing_dir/notify.ior");

my $status = $server->SpawnWaitKill ($target->ProcessStartWaitInterval ());
if ($status != 1) {
    print STDERR "ERROR: expected initialization failure (1), got $status\n";
    exit 1;
}

exit 0;
