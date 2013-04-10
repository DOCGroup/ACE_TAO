eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $service = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$CS = $service->CreateProcess ("NamingMessenger", "-ORBdebuglevel $debug_level");

if ($CS->SpawnWaitKill($service->ProcessStartWaitInterval()) != 0) {
   exit 1;
}

exit 0;
